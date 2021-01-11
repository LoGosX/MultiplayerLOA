#pragma once

#include "SFML/Graphics.hpp"
#include <map>
#include <memory>

#include "networking/windowstcpclient.h"
#include "app/waitforconnectionwindow.h"

class App {
public:
    App();
    void Run();
private:
    enum class AppState {
        kWaitingForConnection,
        kPlaying
    };
    void AppLoop();
    sf::Font font_;
    std::unique_ptr<TCPClient> client_;
    AppState state_;
    std::unique_ptr<WaitForConnectionWindow> connection_;
    Window * current_window_ = nullptr;
};