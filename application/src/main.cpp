#include <iostream>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include "TLC59711.h"
#include "patterns.h"

#define DATA_PIN 17
#define CLK_PIN  27

int main() {
    try {
        TLC59711 tlc(DATA_PIN, CLK_PIN);
        tlc.start();

        // Synchronisation between the completion callback and main.
        // Course Ch. 3.3.2: use a callback to signal thread termination,
        // not polling or sleep.
        std::mutex              mtx;
        std::condition_variable cv;
        std::atomic<bool>       done{false};

        auto onDone = [&]() {
            {
                std::lock_guard<std::mutex> lock(mtx);
                done = true;
            }
            cv.notify_one();
        };

        // --- Fade pattern ---
        std::cout << "Fading in/out...\n";
        PatternFade fade(tlc);
        fade.start(onDone);

        // Block main until the fade finishes via its callback.
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [&]{ return done.load(); });
        }
        std::cout << "Fade complete.\n";

        // --- Ripple pattern (runs indefinitely until stopped) ---
        // std::cout << "Ripple running... (Ctrl-C to stop)\n";
        // PatternRipple ripple(tlc);
        // ripple.start();
        // std::this_thread::sleep_for(std::chrono::seconds(5)); // demo only
        // ripple.stop();

        tlc.stop();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}