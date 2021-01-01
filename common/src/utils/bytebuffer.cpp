#include <memory>

#include "utils/bytebuffer.h"


void ByteBuffer::LoadFrom(uint8_t * src, int bytes) {
    head_ = bytes;
    memcpy(buffer_, src, bytes);
}