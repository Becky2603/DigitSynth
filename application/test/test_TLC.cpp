#include "TLC59711.h"
#include <cstdio>

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

    printf("\n%s\n", status == 0 ? "All tests passed." : "SOME TESTS FAILED.");
    return status == 0 ? 0 : -1;
}