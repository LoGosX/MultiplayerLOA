#include "networking/windowstcpclient.h"
#include "spdlog/spdlog.h"
#include "common/utils/bytebuffer.h"

#include <thread>

WindowsTCPClient::WindowsTCPClient(std::string addr, std::string port) :
    address_(addr), port_(port) {
        InitializeWinsock();

        StartThreads();
    }

void WindowsTCPClient::StartThreads() {
    thread_should_run_ = true;
    std::thread thread(&WindowsTCPClient::WaitForIncomingMessage, this);
    thread.detach();
}

bool WindowsTCPClient::IsConnected() {
    return connected_;
}

bool WindowsTCPClient::CanReceive() {
    return can_receive_;
}

bool WindowsTCPClient::CanSend() {
    spdlog::warn("WindowsTCPClient::CanReceive() not implemented yet");
    return true;
}

void WindowsTCPClient::InitializeWinsock() {
    // Initialize Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        spdlog::error("WSAStartup failed with error: {}\n", iResult);
        connected_ = false;
        return;
    }
}

void WindowsTCPClient::Connect() {
    connect_socket_ = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    int iResult;
    

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(address_.c_str(), port_.c_str(), &hints, &result);
    if ( iResult != 0 ) {
        spdlog::error("getaddrinfo failed with error: {}\n", iResult);
        connected_ = false;
        return;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        this->connect_socket_ = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (this->connect_socket_ == INVALID_SOCKET) {
            spdlog::error("socket failed with error: {}\n", WSAGetLastError());
            connected_ = false;
            return;
        }

        // Connect to server.
        iResult = connect( this->connect_socket_, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(this->connect_socket_);
            this->connect_socket_ = INVALID_SOCKET;
            continue;
        }
        break;
    }
    freeaddrinfo(result);
    if (this->connect_socket_ == INVALID_SOCKET) {
        spdlog::error("Unable to connect to server!\n");
        connected_ = false;
        return;
    }
    connected_ = true;
}

void WindowsTCPClient::Send(const ByteBuffer & buffer) {
    // Send an initial buffer
    int bytesSent = 0;
    while(bytesSent < ByteBuffer::kBufferSize){
        int iResult = send( this->connect_socket_, (char *)buffer.GetBuffer(), buffer.GetSize(), 0 );
        if (iResult == SOCKET_ERROR) {
            spdlog::error("send failed with error: {}\n", WSAGetLastError());
            return;
        }
        bytesSent += iResult;
    }
    spdlog::info("Bytes Sent: {}\n", bytesSent);
}

ByteBuffer WindowsTCPClient::Receive() {
    spdlog::info("Got {} bytes", last_received_buffer_.GetSize());
    can_receive_ = false;
    return last_received_buffer_;
}

void WindowsTCPClient::Close() {
    // shutdown the connection since no more data will be sent
    int iResult = shutdown(this->connect_socket_, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        spdlog::error("shutdown failed with error: {}\n", WSAGetLastError());
    }
    // cleanup
    closesocket(this->connect_socket_);
    WSACleanup();
}


void WindowsTCPClient::WaitForIncomingMessage() {
    can_receive_ = false;
    char buf[1024];
    while(thread_should_run_){
        if(!IsConnected())
            continue;
        int bytesReceived = 0;
        while(bytesReceived < ByteBuffer::kBufferSize) {
            int iResult = recv(this->connect_socket_, buf, 1024, 0);
            if ( iResult > 0 ){
                bytesReceived += iResult;
            }
            else if ( iResult == 0 ){
                spdlog::error("Connection closed in receive\n");
                thread_should_run_ = false;
                can_receive_ = false;
                connected_ = false;
                return;
            }
            else{
                spdlog::error("recv failed with error: {}\n", WSAGetLastError());
                return;
            }
        }
        last_received_buffer_.LoadFrom(buf, bytesReceived);
        can_receive_ = true;
    }
}