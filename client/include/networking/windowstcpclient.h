#pragma once
#include "common/networking/tcpclient.h"

class WindowsTCPClient : public TCPClient {
public:
    using TCPClient::TCPClient;
    bool Connect() override;
    void Close() override;
};