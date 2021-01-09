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
    client->Receive();
    while(true) {
        if(!client->CanReceive())
            continue;
        Message message(client->Receive());
        spdlog::info("Got message of type {}", static_cast<int>(message.type));
        if(message.type == Type::kGameStarted) {
            message.type = Type::kGameStartedAccepted;
            client->Send(message.ToByteBuffer());
        }else if(message.type == Type::kRequestingMove){
            std::cout << message.avaliableMoves.size() << "# moves avaliable. Answering with first move...\n";
            std::cin.get();
            message.type = Type::kSendingMove;
            message.moveMade = message.avaliableMoves.front();
            client->Send(message.ToByteBuffer());
        }else if(message.type == Type::kMoveOK) {
            std::cout << "Move was correct\n";
        }
        Sleep(5000);
    }
    return 0;
}
