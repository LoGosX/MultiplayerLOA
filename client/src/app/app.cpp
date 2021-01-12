#include "app/app.h"

#include <iostream>

#include "spdlog/spdlog.h"

App::App() {
        if(!font_.loadFromFile("UbuntuMono-Regular.ttf")){
            spdlog::error("Could not load font");
        }
        connection_ = std::make_unique<WaitForConnectionWindow>(
            sf::VideoMode(900, 900), "MultiplayerLOA", font_
            );
        playing_ = std::make_unique<PlayingWindow>(
            sf::VideoMode(900, 900), "MultiplayerLOA", font_
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
    if(state_ == AppState::kWaitingForConnection){
        if(connection_->IsReady()) {
            spdlog::info("Trying to connect to {}", connection_->GetIP());
            WindowsTCPClient * client = new WindowsTCPClient(connection_->GetIP(), "1234");
            client_.reset(client);
            timer_.Start(5000);
            state_ = AppState::kTryingToConnect;
        }
    }else if(state_ == AppState::kTryingToConnect) {
        if(!timer_.Finished()) {
            client_->Connect();
        }
        if(client_->IsConnected()) {
            state_ = AppState::kPlaying;
            current_window_->Close();
            current_window_ = playing_.get();
            playing_->Open();
            playing_->SetClient(client_.get());
        }
    }else if(state_ == AppState::kPlaying) {

    }
}