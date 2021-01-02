#pragma once
#include <string>
#include "common/networking/client.h"

class TCPClient : public Client {
public:
    virtual ~TCPClient() = default;
    virtual void Connect() = 0;
    virtual void Close() = 0;
    virtual bool IsConnected() = 0;
};