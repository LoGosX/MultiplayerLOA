#include <iostream>

#include "common/color.h"
#include "networking/linuxtcpclient.h"
#include "spdlog/spdlog.h"

int main(int argc, char const *argv[])
{
    std::cout << "Hello World" << std::endl;
    Color c = Color::kBlack;
    std::cout << static_cast<int>(c) << std::endl;
    TCPClient * client = new LinuxTCPClient(1234, "127.0.0.1");
    std::cout << client->Connect() << std::endl;
    spdlog::info("Welcome to spdlog from main!");
    return 0;
}
