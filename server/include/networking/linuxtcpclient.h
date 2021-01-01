#pragma once
#include "common/networking/tcpclient.h"

class LinuxTCPClient : public TCPClient {
public:
    using TCPClient::TCPClient; //inherit constructor
    bool Connect() override;
    void Close() override;
};