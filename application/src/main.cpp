#include <iostream>
#include <thread>
#include <chrono>
#include "TLC59711.h"

#define DATA_PIN  17
#define CLK_PIN   27

int main() {
    try {
        TLC59711 tlc(DATA_PIN, CLK_PIN);
        tlc.start();
        std::cout << "TLC59711 started\n";

        const int   STEPS   = 200;
        const int   STEP_MS = 10;

        // Fade each group up in turn, then all together, then back down.
        // LEDFrame indices: [R1 R2 R3 R4  G1 G2 G3  B1 B2 B3]
        struct Phase { const char* label; bool r, g, b; };
        const Phase phases[] = {
            { "Reds up",   true,  false, false },
            { "Greens up", false, true,  false },
            { "Blues up",  false, false, true  },
            { "All up",    true,  true,  true  },
            { "All down",  true,  true,  true  },
        };

        for (const auto& phase : phases) {
            std::cout << phase.label << "\n";
            const bool down = (std::string(phase.label).find("down") != std::string::npos);

            for (int step = 0; step <= STEPS; ++step) {
                const float t = static_cast<float>(down ? (STEPS - step) : step)
                                / static_cast<float>(STEPS);

                LEDFrame frame{};
                // R1–R4
                if (phase.r)
                    for (int i = 0; i < 4; ++i) frame.channels[i] = t;
                // G1–G3
                if (phase.g)
                    for (int i = 4; i < 7; ++i) frame.channels[i] = t;
                // B1–B3
                if (phase.b)
                    for (int i = 7; i < 10; ++i) frame.channels[i] = t;

                tlc.update(frame);
                std::this_thread::sleep_for(std::chrono::milliseconds(STEP_MS));
            }
        }

        tlc.stop();
        std::cout << "Done\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}