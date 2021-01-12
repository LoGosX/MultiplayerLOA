#pragma once

#include <chrono>

class Timer {

public:
    Timer() = default;
    Timer(int ms);
    
    void Start(int ms);
    bool Finished() const;

private:
    std::chrono::high_resolution_clock::time_point start_;
    int ms_;
};