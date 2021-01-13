#pragma once
#include <cstdint>
#include <string>
#include <vector>

class ByteBuffer {
    //using Byte = uint8_t;
    using Byte = char;
public:

    inline static const int kBufferSize = 256;

    ByteBuffer() = default;
    ByteBuffer(const std::string &);

    bool IsEmpty() const;
    void LoadFrom(Byte * source, int bytes_count);
    const Byte * GetBuffer() const;
    int GetSize() const;

    void WriteByte(Byte);
    void WriteChar(char);
    void WriteString(const std::string&);

    Byte ReadByte();
    char ReadChar();
    std::string ReadString(int n_chars = -1);
    std::string ReadStringUntil(char terminal);

    std::string ToString() const;

    void PadToBufferSize();
private:
    int tail_ = 0;
    std::vector<Byte> buffer_;
};