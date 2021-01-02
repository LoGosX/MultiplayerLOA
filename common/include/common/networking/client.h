#pragma once
#include "common/utils/bytebuffer.h"

class Client {
public:
    virtual ~Client() = default;
    virtual void Send(ByteBuffer) = 0;
    virtual ByteBuffer Receive() = 0;
    virtual bool CanReceive() = 0;
};