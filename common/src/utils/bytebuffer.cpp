#include <memory>
#include "utils/bytebuffer.h"

ByteBuffer::ByteBuffer(unsigned int size) : kSize(size) {
    buffer_ = new uint8_t[size];
}

ByteBuffer::~ByteBuffer() {
    delete buffer_;
}

ByteBuffer::ByteBuffer(const ByteBuffer & copy) : kSize(copy.kSize) {
    head_ = copy.head_;
    buffer_ = new uint8_t[kSize];
    memcpy(buffer_, copy.buffer_, kSize);
}

void ByteBuffer::LoadFrom(uint8_t * src, int bytes) {
    head_ = bytes;
    memcpy(buffer_, src, bytes);
}

uint8_t * ByteBuffer::GetBuffer() const {
    return buffer_;
}

int ByteBuffer::GetSize() const {
    return head_ - tail_;
}

void ByteBuffer::SetSize(int s) {
    head_ = s;
    tail_ = 0;
}

bool ByteBuffer::IsEmpty() const {
    return head_ == tail_;
}