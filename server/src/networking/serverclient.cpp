#pragma once

#include "networking/serverclient.h"

#include "spdlog/spdlog.h"

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
    spdlog::info("Sending buffer {}", buffer.ToString());
    write(fd_, buffer.GetBuffer(), buffer.GetSize());
    spdlog::info("Buffer sent");
}


ByteBuffer ServerClient::Receive() {
    char buffer[1024];
    spdlog::info("Reading bytes when can_receive={}", can_receive_);
    int n = read(fd_, buffer, 1024);
    spdlog::info("Got {} bytes", n);
    ByteBuffer buf;
    buf.LoadFrom(buffer, n);
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