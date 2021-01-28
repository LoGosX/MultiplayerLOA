#pragma once
#include "common/networking/client.h"

class ServerClient : public Client {
public:

    ServerClient(int fd);

    void Send(const ByteBuffer &) override;
    ByteBuffer Receive() override;
    bool CanReceive() override;
    bool CanSend() override;

    void SetCanReceive(bool);
    void SetCanSend(bool);

    int GetFd() const;

    bool IsValid() const;
    void Invalidate();

private:
    bool valid_ = true;
    int fd_;
    bool can_receive_ = false;
    bool can_send_ = false;
};