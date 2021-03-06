#pragma once
#include "common/networking/tcpclient.h"

#include <atomic>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


class WindowsTCPClient : public TCPClient {
public:
    WindowsTCPClient(std::string address, std::string port);

    void Connect() override;
    void Close() override;
    bool IsConnected() override;

    ByteBuffer Receive() override;
    void Send(const ByteBuffer &) override;
    bool CanReceive() override;
    bool CanSend() override;

    void InitializeWinsock();

private:
    void WaitForIncomingMessage();
    void StartThreads();
    std::atomic<bool> connected_ = false;
    std::string address_;
    std::string port_;
    SOCKET connect_socket_;
    std::atomic<bool> can_receive_ = false;
    std::atomic<bool> thread_should_run_ = false;
    ByteBuffer last_received_buffer_;
};