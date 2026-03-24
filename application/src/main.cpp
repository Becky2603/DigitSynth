#include <iostream>
#include <thread>
#include <chrono>
#include <array>
#include <stdexcept>
#include <cstdint>
#include <cmath>

#include <gpiod.h>
#include <time.h>

#define DATA_PIN  17
#define CLK_PIN   27

class TLC59711 {
public:
    using GSArray = std::array<uint16_t, 12>;

    static constexpr uint8_t BC_MAX         = 0x7F;
    static constexpr long    HALF_PERIOD_NS = 5000L;

    TLC59711()
    {
        for (const char* path : { "/dev/gpiochip0", "/dev/gpiochip4" }) {
            chip_ = gpiod_chip_open(path);
            if (chip_) { std::cout << "Opened " << path << "\n"; break; }
        }
        if (!chip_)
            throw std::runtime_error("Cannot open gpiochip — try: sudo apt upgrade && reboot");

        gpiod_line_settings* settings = gpiod_line_settings_new();
        if (!settings) throw std::runtime_error("gpiod_line_settings_new failed");

        gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
        gpiod_line_settings_set_output_value(settings, GPIOD_LINE_VALUE_INACTIVE);

        gpiod_line_config* cfg = gpiod_line_config_new();
        if (!cfg) {
            gpiod_line_settings_free(settings);
            throw std::runtime_error("gpiod_line_config_new failed");
        }

        const unsigned int offsets[2] = { DATA_PIN, CLK_PIN };
        if (gpiod_line_config_add_line_settings(cfg, offsets, 2, settings) < 0) {
            gpiod_line_config_free(cfg);
            gpiod_line_settings_free(settings);
            throw std::runtime_error("gpiod_line_config_add_line_settings failed");
        }

        request_ = gpiod_chip_request_lines(chip_, nullptr, cfg);

        gpiod_line_config_free(cfg);
        gpiod_line_settings_free(settings);

        if (!request_)
            throw std::runtime_error("gpiod_chip_request_lines failed — are the pins free?");
    }

    ~TLC59711()
    {
        if (request_) {
            gpiod_line_request_set_value(request_, DATA_PIN, GPIOD_LINE_VALUE_INACTIVE);
            gpiod_line_request_set_value(request_, CLK_PIN,  GPIOD_LINE_VALUE_INACTIVE);
            gpiod_line_request_release(request_);
        }
        if (chip_) gpiod_chip_close(chip_);
    }

    TLC59711(const TLC59711&)            = delete;
    TLC59711& operator=(const TLC59711&) = delete;

    void write(const GSArray& gs,
               uint8_t bcR = BC_MAX, uint8_t bcG = BC_MAX, uint8_t bcB = BC_MAX)
    {
        shiftOut(buildFrame(gs, bcR, bcG, bcB));
    }

    void clear() { write(GSArray{}); }

private:
    using Frame = std::array<uint8_t, 28>;

    static Frame buildFrame(const GSArray& gs, uint8_t bcR, uint8_t bcG, uint8_t bcB)
    {
        Frame f{};

        uint32_t cmd = 0;
        cmd |= uint32_t{0x25}        << 26;
        cmd |= uint32_t{1}           << 25;  // OUTTMG
        cmd |= uint32_t{0}           << 24;  // EXTGCK
        cmd |= uint32_t{1}           << 23;  // TMGRST
        cmd |= uint32_t{1}           << 22;  // DSPRPT
        cmd |= uint32_t{0}           << 21;  // BLANK
        cmd |= uint32_t{bcB & 0x7Fu} << 14;
        cmd |= uint32_t{bcG & 0x7Fu} <<  7;
        cmd |= uint32_t{bcR & 0x7Fu};

        f[0] = static_cast<uint8_t>(cmd >> 24);
        f[1] = static_cast<uint8_t>(cmd >> 16);
        f[2] = static_cast<uint8_t>(cmd >>  8);
        f[3] = static_cast<uint8_t>(cmd      );

        size_t bi = 4;
        for (int led = 3; led >= 0; --led) {
            for (int ch = 2; ch >= 0; --ch) {
                uint16_t v = gs[static_cast<size_t>(led * 3 + ch)];
                f[bi++] = static_cast<uint8_t>(v >> 8);
                f[bi++] = static_cast<uint8_t>(v     );
            }
        }
        return f;
    }

    inline void setData(int v) const {
        gpiod_line_request_set_value(request_, DATA_PIN,
            v ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE);
    }
    inline void setCLK(int v) const {
        gpiod_line_request_set_value(request_, CLK_PIN,
            v ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE);
    }
    static void nsleep(long ns) {
        struct timespec ts{ 0, ns };
        nanosleep(&ts, nullptr);
    }

    void shiftOut(const Frame& frame) const
    {
        for (uint8_t byte : frame) {
            for (int bit = 7; bit >= 0; --bit) {
                setData((byte >> bit) & 1);
                nsleep(HALF_PERIOD_NS);
                setCLK(1);
                nsleep(HALF_PERIOD_NS);
                setCLK(0);
            }
        }
        setData(0);
        nsleep(HALF_PERIOD_NS * 20);
    }

    gpiod_chip*         chip_    = nullptr;
    gpiod_line_request* request_ = nullptr;
};

// ── Gamma-correct a linear 0.0–1.0 value to a 16-bit GS count ──────────────
//
// Human vision is roughly logarithmic. A naive linear ramp from 0→65535 looks
// like it jumps bright immediately then barely changes. Applying a gamma curve
// (typically γ=2.2 matching sRGB) redistributes the steps so equal increments
// in the ramp feel like equal increments in perceived brightness.

static uint16_t gamma16(float t, float gamma = 2.2f)
{
    if (t <= 0.0f) return 0;
    if (t >= 1.0f) return 0xFFFF;
    return static_cast<uint16_t>(std::pow(t, gamma) * 65535.0f + 0.5f);
}

int main() {
    try {
        TLC59711 tlc;
        std::cout << "TLC59711 initialised\n";

        const int   STEPS    = 200;          // steps per fade
        const int   STEP_MS  = 10;           // ms per step → 2 s per fade
        const float GAMMA    = 2.2f;

        // Fade R0, G0, B0 up one at a time, then all together, then back down
        struct Phase {
            const char* label;
            bool r, g, b;
        };
        const Phase phases[] = {
            { "R0 up",   true,  false, false },
            { "G0 up",   false, true,  false },
            { "B0 up",   false, false, true  },
            { "all up",  true,  true,  true  },
            { "all down",true,  true,  true  },  // fades back down
        };

        for (const auto& phase : phases) {
            std::cout << phase.label << "\n";

            bool fadeDown = (std::string(phase.label).find("down") != std::string::npos);

            for (int step = 0; step <= STEPS; ++step) {
                float t = static_cast<float>(fadeDown ? (STEPS - step) : step)
                          / static_cast<float>(STEPS);

                uint16_t val = gamma16(t, GAMMA);

                TLC59711::GSArray gs{};
                if (phase.r) gs[0] = val;   // R0
                if (phase.g) gs[1] = val;   // G0
                if (phase.b) gs[2] = val;   // B0

                tlc.write(gs);
                std::this_thread::sleep_for(std::chrono::milliseconds(STEP_MS));
            }
        }

        tlc.clear();
        std::cout << "Done\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}