#include "app/app.h"

#include <iostream>

#include "spdlog/spdlog.h"

#include "common/networking/message.h"

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
        connection_->Reset("Podaj dane");
        current_window_ = connection_.get();
    }

void App::Run() {
    state_ = AppState::kWaitingForConnection;
    current_window_->Open();
    while(current_window_->IsOpen()) {
        AppLoop();
        current_window_->PoolEvents();
        current_window_->Update();
    }
}

void App::AppLoop() {
    if(state_ == AppState::kWaitingForConnection){
        if(connection_->IsReady()) {
            spdlog::info("Trying to connect to {}", connection_->GetServerIP());
            WindowsTCPClient * client = new WindowsTCPClient(connection_->GetServerIP(), "1234");
            client_.reset(client);
            timer_.Start(5000);
            state_ = AppState::kTryingToConnect;
        }
    }else if(state_ == AppState::kTryingToConnect) {
        if(!timer_.Finished()) {
            connection_->SetMessage("Trying to connect...");
            client_->Connect();
        }else{
            connection_->Reset("Could not connect to server.");
            current_window_ = connection_.get();
            state_ = AppState::kWaitingForConnection;
        }
        if(client_->IsConnected()) {
            Message message;
            message.type = Type::kSearchingForGame;
            message.opponentIP = connection_->GetClientIP();
            client_->Send(message.ToByteBuffer());
            state_ = AppState::kPlaying;
            current_window_->Close();
            current_window_ = playing_.get();
            playing_->Open();
            playing_->SetClient(client_.get());
        }
    }else if(state_ == AppState::kPlaying) {
        /*
        if(playing_->IsOK()) {
            connection_->Reset(playing_->GetError());
            current_window_ = connection_;
            state_ = AppState::kTryingToConnect;
        }
        */
    }
}