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
    message.type = Type::kSendingName;
    message.name = "inf141210";
    client->Send(message.ToByteBuffer());
    ByteBuffer buffer = client->Receive();
    message.FromByteBuffer(buffer);
    std::cout << message.ToString() << std::endl;
    return 0;
}
