#include <iostream>

#include "common/game/color.h"
#include "common/networking/message.h"
#include "common/utils/bytebuffer.h"
#include "spdlog/spdlog.h"

#include "networking/windowstcpclient.h"

#include "windows.h" 

#include "SFML/Graphics.hpp"

#include "app/app.h"

void sfmlTest() {
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }
}

void windowTest() {
    App app;
    app.Run();
}

int main(int argc, char const *argv[])
{
    windowTest();
    sfmlTest();
    spdlog::info("sizeof(char)={}",sizeof(char));
    TCPClient * client = new WindowsTCPClient("127.0.0.1", "1234");
    while(!client->IsConnected())
        client->Connect();
    client->Receive();
    while(true) {
        if(!client->CanReceive())
            continue;
        auto buffer = client->Receive();
        spdlog::info("Got buffer {}", buffer.ToString());
        Message message(buffer);

        spdlog::info("Got {}", message.ToString());
        if(message.type == Type::kGameStarted) {
            message.type = Type::kGameStartedAccepted;
            client->Send(message.ToByteBuffer());
            spdlog::info("Sending {}", message.ToString());
        }else if(message.type == Type::kRequestingMove){
            std::cout << message.avaliableMoves.size() << "# moves avaliable. Answering with first move...\n";
            std::cin.get();
            message.type = Type::kSendingMove;
            message.moveMade = message.avaliableMoves.front();
            spdlog::info("Sending {}", message.ToString());
            client->Send(message.ToByteBuffer());
        }else if(message.type == Type::kMoveOK) {
            std::cout << "Move was correct\n";
        }
        Sleep(5000);
    }
    return 0;
}
