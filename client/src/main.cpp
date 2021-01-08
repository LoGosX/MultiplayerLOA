#include <iostream>

#include "common/game/color.h"
#include "common/networking/message.h"
#include "common/utils/bytebuffer.h"
#include "spdlog/spdlog.h"

#include "networking/windowstcpclient.h"

#include "windows.h" 

int main(int argc, char const *argv[])
{
    TCPClient * client = new WindowsTCPClient("127.0.0.1", "1234");
    while(!client->IsConnected())
    client->Connect();
    Message message;
    message.type = Type::kSearchingForGame;
    message.name = "white";
    message.opponentName = "black";
    client->Send(message.ToByteBuffer());
    ByteBuffer buffer = client->Receive();
    std::cout << "ByteBuffer size: " << buffer.GetSize();
    message.FromByteBuffer(buffer);
    std::cout << "Got: " << message.ToString() << std::endl;
    client->Close();
    return 0;
}
