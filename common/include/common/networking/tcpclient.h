#pragma once
#include <string>

class TCPClient {

public:
    TCPClient(unsigned int port, std::string addres);
    virtual ~TCPClient() = default;
    virtual bool Connect() = 0;
    virtual void Close() = 0;
protected:
    int fd_;
    unsigned int port_;
    std::string addres_;
};