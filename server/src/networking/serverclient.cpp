#pragma once

#include "networking/serverclient.h"


#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>


ServerClient::ServerClient(int fd) : fd_(fd) {}

void ServerClient::SetCanReceive(bool v) {
    can_receive_ = v;
}

void ServerClient::SetCanSend(bool v) {
    can_send_ = v;
}


void ServerClient::Send(const ByteBuffer & buffer) {
    int bytesSent = 0;
    while(bytesSent < ByteBuffer::kBufferSize){
        int w = write(fd_, buffer.GetBuffer() + bytesSent, buffer.GetSize() - bytesSent);
        bytesSent += w;
    }
}


ByteBuffer ServerClient::Receive() {
    char buffer[1024];
    int bytesReceived = 0;
    while(bytesReceived < ByteBuffer::kBufferSize) {
        int n = read(fd_, buffer, 1024);
        bytesReceived += n;
    }
    ByteBuffer buf;
    buf.LoadFrom(buffer, bytesReceived);
    can_receive_ = false;
    return buf;
}

bool ServerClient::CanReceive() {
    return can_receive_;
}

bool ServerClient::CanSend() {
    return can_send_;
}

int ServerClient::GetFd() const {
    return fd_;
}

bool ServerClient::IsValid() const {
    return valid_;
}

void ServerClient::Invalidate() {
    valid_ = false;
}