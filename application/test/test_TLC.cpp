#include "TLC59711.h"
#include <cstdio>
#include <mutex>
#include <thread>
#include <atomic>

// ============================================================================
// TLC59711Test — friend class declared in TLC59711.h, giving access to all
// private members so we can test internal state directly without GPIO hardware.
// ============================================================================

class TLC59711Test {
public:

    // -----------------------------------------------------------------------
    // toGS() — brightness-to-grayscale conversion
    // -----------------------------------------------------------------------

    static int test_toGS_zero() {
        return TLC59711::toGS(0.0f) == 0 ? 0 : -1;
    }

    static int test_toGS_full() {
        return TLC59711::toGS(1.0f) == 0xFFFF ? 0 : -1;
    }

    static int test_toGS_clamp_negative() {
        return TLC59711::toGS(-1.0f) == 0 ? 0 : -1;
    }

    static int test_toGS_clamp_over() {
        return TLC59711::toGS(2.0f) == 0xFFFF ? 0 : -1;
    }

    static int test_toGS_midpoint() {
        const uint16_t gs = TLC59711::toGS(0.5f);
        return (gs > 0 && gs <= 0x8000) ? 0 : -1;
    }

    static int test_toGS_monotonic() {
        const uint16_t lo = TLC59711::toGS(0.25f);
        const uint16_t hi = TLC59711::toGS(0.75f);
        return (hi >= lo) ? 0 : -1;
    }

    static int test_toGS_quarter() {
        const uint16_t gs = TLC59711::toGS(0.25f);
        return (gs > 0 && gs < 0xFFFF) ? 0 : -1;
    }

    // -----------------------------------------------------------------------
    // FRAME_TO_GS — LED-to-channel mapping sanity
    // -----------------------------------------------------------------------

    static int test_channel_map_unique() {
        for (int i = 0; i < TLC59711::NUM_LEDS; ++i)
            for (int j = i + 1; j < TLC59711::NUM_LEDS; ++j)
                if (TLC59711::FRAME_TO_GS[i] == TLC59711::FRAME_TO_GS[j])
                    return -1;
        return 0;
    }

    static int test_channel_map_in_range() {
        for (int i = 0; i < TLC59711::NUM_LEDS; ++i)
            if (TLC59711::FRAME_TO_GS[i] < 0 ||
                TLC59711::FRAME_TO_GS[i] >= TLC59711::CHANNELS_PER_DRIVER)
                return -1;
        return 0;
    }

    // -----------------------------------------------------------------------
    // update() — non-blocking, stores pending state
    // -----------------------------------------------------------------------

    static int test_update_returns_immediately() {
        TLC59711 tlc(0, 0);

        TLC59711::Channels ch{};
        for (int i = 0; i < TLC59711::NUM_LEDS; ++i) ch[i] = 0.5f;

        const auto t0 = std::chrono::steady_clock::now();
        tlc.update(ch);
        const auto elapsed_us = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::steady_clock::now() - t0).count();

        return (elapsed_us < 10000) ? 0 : -1;
    }

    static int test_update_stores_pending() {
        TLC59711 tlc(0, 0);

        TLC59711::Channels ch{};
        for (int i = 0; i < TLC59711::NUM_LEDS; ++i)
            ch[i] = static_cast<float>(i) / TLC59711::NUM_LEDS;

        tlc.update(ch);

        std::lock_guard<std::mutex> lock(tlc._mutex);
        if (!tlc._dirty) return -1;
        for (int i = 0; i < TLC59711::NUM_LEDS; ++i)
            if (tlc._pending[i] != ch[i]) return -1;

        return 0;
    }

    static int test_update_overwrites_previous_pending() {
        TLC59711 tlc(0, 0);

        TLC59711::Channels first{};
        TLC59711::Channels second{};
        for (int i = 0; i < TLC59711::NUM_LEDS; ++i) {
            first[i]  = 0.25f;
            second[i] = 0.75f;
        }

        tlc.update(first);
        tlc.update(second);

        std::lock_guard<std::mutex> lock(tlc._mutex);
        for (int i = 0; i < TLC59711::NUM_LEDS; ++i)
            if (tlc._pending[i] != 0.75f) return -1;

        return 0;
    }

    static int test_update_dirty_flag_set() {
        TLC59711 tlc(0, 0);
        {
            std::lock_guard<std::mutex> lock(tlc._mutex);
            tlc._dirty = false;
        }
        TLC59711::Channels ch{};
        tlc.update(ch);

        std::lock_guard<std::mutex> lock(tlc._mutex);
        return tlc._dirty ? 0 : -1;
    }

    static int test_update_all_channels_stored() {
        TLC59711 tlc(0, 0);

        TLC59711::Channels ch{};
        ch[0] = 0.1f;
        ch[4] = 0.4f;
        ch[9] = 0.9f;

        tlc.update(ch);

        std::lock_guard<std::mutex> lock(tlc._mutex);
        if (tlc._pending[0] != 0.1f) return -1;
        if (tlc._pending[4] != 0.4f) return -1;
        if (tlc._pending[9] != 0.9f) return -1;
        return 0;
    }

    // -----------------------------------------------------------------------
    // update() — concurrent writers must not corrupt _pending
    //
    // Two threads each call update() 1000 times with a consistent all-same-
    // value Channels array. After both finish, _pending must still contain
    // an all-same-value snapshot — no partial write from a torn copy.
    // If _mutex were missing, this would regularly produce mixed arrays.
    // -----------------------------------------------------------------------

    static int test_update_thread_safe() {
        TLC59711 tlc(0, 0);

        std::atomic<bool> go{false};

        auto writer = [&](float val) {
            while (!go) {}   // spin until both threads are ready
            TLC59711::Channels ch{};
            for (int rep = 0; rep < 1000; ++rep) {
                for (auto& v : ch) v = val;
                tlc.update(ch);
            }
        };

        std::thread t1(writer, 0.2f);
        std::thread t2(writer, 0.8f);
        go = true;
        t1.join();
        t2.join();

        std::lock_guard<std::mutex> lock(tlc._mutex);
        if (!tlc._dirty) return -1;

        // Every entry must match the first — no torn write across channels
        const float expected = tlc._pending[0];
        for (int i = 1; i < TLC59711::NUM_LEDS; ++i)
            if (tlc._pending[i] != expected) return -1;

        return 0;
    }

    // -----------------------------------------------------------------------
    // setBrightness()
    // -----------------------------------------------------------------------

    static int test_setBrightness_default() {
        TLC59711 tlc(0, 0);
        return (tlc._brightness == 127) ? 0 : -1;
    }

    static int test_setBrightness_stores_value() {
        TLC59711 tlc(0, 0);
        tlc.setBrightness(200);
        return (tlc._brightness == 200) ? 0 : -1;
    }

    static int test_setBrightness_zero() {
        TLC59711 tlc(0, 0);
        tlc.setBrightness(0);
        return (tlc._brightness == 0) ? 0 : -1;
    }

    static int test_setBrightness_max() {
        TLC59711 tlc(0, 0);
        tlc.setBrightness(255);
        return (tlc._brightness == 255) ? 0 : -1;
    }

    // -----------------------------------------------------------------------
    // buildPacket() — SPI packet structure
    // -----------------------------------------------------------------------

    static int test_buildPacket_size_one_driver() {
        TLC59711 tlc(0, 0, 1);
        std::vector<uint8_t> buf;
        tlc.buildPacket(buf);
        return (buf.size() == 28) ? 0 : -1;
    }

    static int test_buildPacket_size_two_drivers() {
        TLC59711 tlc(0, 0, 2);
        std::vector<uint8_t> buf;
        tlc.buildPacket(buf);
        return (buf.size() == 56) ? 0 : -1;
    }

    static int test_buildPacket_magic_bits() {
        TLC59711 tlc(0, 0, 1);
        std::vector<uint8_t> buf;
        tlc.buildPacket(buf);
        const uint8_t top6 = buf[0] >> 2;
        return (top6 == 0x25) ? 0 : -1;
    }

    static int test_buildPacket_all_zero_gs_when_dark() {
        TLC59711 tlc(0, 0, 1);
        std::vector<uint8_t> buf;
        tlc.buildPacket(buf);
        for (size_t i = 4; i < buf.size(); ++i)
            if (buf[i] != 0x00) return -1;
        return 0;
    }

    static int test_buildPacket_brightness_in_cmd() {
        TLC59711 tlc(0, 0, 1);
        tlc.setBrightness(0x3F);

        std::vector<uint8_t> buf;
        tlc.buildPacket(buf);

        const uint32_t cmd = (uint32_t(buf[0]) << 24) |
                             (uint32_t(buf[1]) << 16) |
                             (uint32_t(buf[2]) <<  8) |
                              uint32_t(buf[3]);

        const uint8_t bcr = (cmd >>  0) & 0x7F;
        const uint8_t bcg = (cmd >>  7) & 0x7F;
        const uint8_t bcb = (cmd >> 14) & 0x7F;
        const uint8_t bc  = 0x3F & 0x7F;

        return (bcr == bc && bcg == bc && bcb == bc) ? 0 : -1;
    }

    // -----------------------------------------------------------------------
    // Thread lifecycle
    // -----------------------------------------------------------------------

    static int test_initial_running_false() {
        TLC59711 tlc(0, 0);
        return (!tlc._running) ? 0 : -1;
    }

    static int test_stop_without_start_safe() {
        TLC59711 tlc(0, 0);
        tlc.stop();
        return 0;
    }

    static int test_destructor_without_start_safe() {
        { TLC59711 tlc(0, 0); }
        return 0;
    }

    static int test_num_leds_constant() {
        return (TLC59711::NUM_LEDS == 10) ? 0 : -1;
    }

    static int test_channels_per_driver_constant() {
        return (TLC59711::CHANNELS_PER_DRIVER == 12) ? 0 : -1;
    }

};


// ============================================================================
// Test runner
// ============================================================================

static int run(const char* name, int(*test)()) {
    const int result = test();
    printf("[%s] %s\n", result == 0 ? "PASS" : "FAIL", name);
    return result;
}

int main(int argc, char** argv) {
    (void)argc; (void)argv;

    int status = 0;

    printf("\n--- toGS: brightness -> grayscale conversion ---\n");
    status |= run("toGS_zero",           TLC59711Test::test_toGS_zero);
    status |= run("toGS_full",           TLC59711Test::test_toGS_full);
    status |= run("toGS_clamp_negative", TLC59711Test::test_toGS_clamp_negative);
    status |= run("toGS_clamp_over",     TLC59711Test::test_toGS_clamp_over);
    status |= run("toGS_midpoint",       TLC59711Test::test_toGS_midpoint);
    status |= run("toGS_monotonic",      TLC59711Test::test_toGS_monotonic);
    status |= run("toGS_quarter",        TLC59711Test::test_toGS_quarter);

    printf("\n--- FRAME_TO_GS: LED-to-channel mapping ---\n");
    status |= run("channel_map_unique",   TLC59711Test::test_channel_map_unique);
    status |= run("channel_map_in_range", TLC59711Test::test_channel_map_in_range);

    printf("\n--- update(): non-blocking, stores pending state ---\n");
    status |= run("update_returns_immediately",  TLC59711Test::test_update_returns_immediately);
    status |= run("update_stores_pending",       TLC59711Test::test_update_stores_pending);
    status |= run("update_overwrites_previous",  TLC59711Test::test_update_overwrites_previous_pending);
    status |= run("update_dirty_flag_set",       TLC59711Test::test_update_dirty_flag_set);
    status |= run("update_all_channels_stored",  TLC59711Test::test_update_all_channels_stored);

    printf("\n--- update(): concurrent writers ---\n");
    status |= run("update_thread_safe",          TLC59711Test::test_update_thread_safe);

    printf("\n--- setBrightness() ---\n");
    status |= run("setBrightness_default",       TLC59711Test::test_setBrightness_default);
    status |= run("setBrightness_stores_value",  TLC59711Test::test_setBrightness_stores_value);
    status |= run("setBrightness_zero",          TLC59711Test::test_setBrightness_zero);
    status |= run("setBrightness_max",           TLC59711Test::test_setBrightness_max);

    printf("\n--- buildPacket(): SPI packet structure ---\n");
    status |= run("buildPacket_size_one_driver",  TLC59711Test::test_buildPacket_size_one_driver);
    status |= run("buildPacket_size_two_drivers", TLC59711Test::test_buildPacket_size_two_drivers);
    status |= run("buildPacket_magic_bits",       TLC59711Test::test_buildPacket_magic_bits);
    status |= run("buildPacket_all_zero_gs_dark", TLC59711Test::test_buildPacket_all_zero_gs_when_dark);
    status |= run("buildPacket_brightness_in_cmd",TLC59711Test::test_buildPacket_brightness_in_cmd);

    printf("\n--- Thread lifecycle ---\n");
    status |= run("initial_running_false",        TLC59711Test::test_initial_running_false);
    status |= run("stop_without_start_safe",      TLC59711Test::test_stop_without_start_safe);
    status |= run("destructor_without_start_safe",TLC59711Test::test_destructor_without_start_safe);
    status |= run("num_leds_constant",            TLC59711Test::test_num_leds_constant);
    status |= run("channels_per_driver_constant", TLC59711Test::test_channels_per_driver_constant);

    printf("\n%s\n", status == 0 ? "All tests passed." : "SOME TESTS FAILED.");
    return status == 0 ? 0 : -1;
}