#include <iostream>

#include "common/game/color.h"
#include "spdlog/spdlog.h"

#include "networking/windowstcpclient.h"

#include "windows.h" 

int main(int argc, char const *argv[])
{
    TCPClient * client = new WindowsTCPClient("127.0.0.1", "1234");
    while(!client->IsConnected())
    client->Connect();
    ByteBuffer buffer;
    buffer.WriteString("inf141210");
    client->Send(buffer);
    buffer = client->Receive();
    std::cout << "Got " << buffer.ReadString();
    return 0;
}
