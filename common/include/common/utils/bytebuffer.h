#pragma once
#include <cstdint>

class ByteBuffer {
public:
    ByteBuffer(unsigned int size=1024);
    ~ByteBuffer();
    bool IsEmpty() const;
    const unsigned int kSize;
    void LoadFrom(uint8_t * source, int bytes_count);
    uint8_t * GetBuffer() const;
    int GetSize() const;
    void SetSize(int);

    ByteBuffer(const ByteBuffer &);
    ByteBuffer(ByteBuffer &&) = default;

private:
    int head_ = 0;
    int tail_ = 0;
    uint8_t * buffer_ = nullptr;
};