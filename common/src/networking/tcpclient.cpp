#include "networking/tcpclient.h"
#include "spdlog/spdlog.h"

TCPClient::TCPClient(unsigned int port, std::string addres) : 
    port_(port), addres_(addres) {
        spdlog::info("Welcome to spdlog from TCPClient!");
    };