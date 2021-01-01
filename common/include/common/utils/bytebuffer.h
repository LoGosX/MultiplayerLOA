#pragma once

#include <vector>
#include <cstdint>

class ByteBuffer {
public:
    ByteBuffer(unsigned int size=1024);
    bool IsEmpty() const;
    const unsigned int kSize;
    void LoadFrom(uint8_t * source, int bytes_count);
private:
    int head_ = 0;
    int tail_ = 0;
    uint8_t * buffer_ = nullptr;
};