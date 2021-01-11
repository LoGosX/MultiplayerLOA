#include "app/app.h"

#include <iostream>

#include "spdlog/spdlog.h"

App::App() {
        if(!font_.loadFromFile("UbuntuMono-Regular.ttf")){
            spdlog::error("Could not load font");
        }
        connection_ = std::make_unique<WaitForConnectionWindow>(
            sf::VideoMode(1200, 900), "MultiplayerLOA", font_
            );
        current_window_ = connection_.get();
    }

void App::Run() {
    state_ = AppState::kWaitingForConnection;
    current_window_->Open();
    while(current_window_->IsOpen()) {
        current_window_->PoolEvents();
        current_window_->Update();
        AppLoop();
    }
}

void App::AppLoop() {
    //std::cout << connection_.GetIP() << std::endl;
    //std::cout << connection_.IsReady() << std::endl;
    if(state_ == AppState::kWaitingForConnection){
    }
}
