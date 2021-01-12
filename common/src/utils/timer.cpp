#include "utils/timer.h"

Timer::Timer(int ms) : ms_(ms) {
}

void Timer::Start(int ms) {
    ms_ = ms;
    start_ = std::chrono::high_resolution_clock::now();
}

bool Timer::Finished() const {
    auto now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now - start_).count() >= ms_;
}