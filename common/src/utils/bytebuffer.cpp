#include <memory>
#include "utils/bytebuffer.h"
#include "spdlog/spdlog.h"
#include <iostream>

ByteBuffer::ByteBuffer(const std::string & s) {
    WriteString(s);
}

void ByteBuffer::LoadFrom(Byte * src, int bytes) {
    buffer_ = std::vector<Byte>(src, src + bytes);
    tail_ = 0;
}

const ByteBuffer::Byte * ByteBuffer::GetBuffer() const {
    return buffer_.data() + tail_;
}

int ByteBuffer::GetSize() const {
    return buffer_.size() - tail_;
}

bool ByteBuffer::IsEmpty() const {
    return buffer_.size() == tail_;
}

void ByteBuffer::WriteByte(Byte byte) {
    buffer_.push_back(byte);
}

void ByteBuffer::WriteChar(char chr) {
    WriteByte(static_cast<Byte>(chr));
}

void ByteBuffer::WriteString(const std::string & s) {
    for(char chr : s)
        WriteChar(chr);
}

ByteBuffer::Byte ByteBuffer::ReadByte() {
    if(IsEmpty()) {
        spdlog::error("Trying to read from empty buffer!");
        return 0;
    }else {
        return buffer_[tail_++];
    }
}

char ByteBuffer::ReadChar() {
    return static_cast<char>(ReadByte());
}

std::string ByteBuffer::ReadString(int n_chars) {
    std::string result;
    for(int i = 0, size = GetSize(); i < size && (i < n_chars || n_chars == -1); i++) {
        result += ReadChar();
    }
    return result;
}

std::string ByteBuffer::ReadStringUntil(char terminal) {
    std::string result;
    for(int i = 0; i < GetSize(); i++) {
        auto chr = ReadChar();
        if(chr == terminal){
            break;
        }
        result += chr;
    }
    return result;
}

std::string ByteBuffer::ToString() const {
    std::string result;
    for(int i = 0; i < GetSize(); i++) {
        result += std::to_string(buffer_[tail_ + i]) + ' ';
    }
    return result;
}