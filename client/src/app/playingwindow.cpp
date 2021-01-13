#pragma once

#include "spdlog/spdlog.h"
#include <iostream>

#include "app/playingwindow.h"
#include "common/networking/message.h"

#include "common/networking/tcpclient.h"

void PlayingWindow::Update() {
    if(client_->CanReceive()){
        auto buffer = client_->Receive();
        spdlog::info("Got buffer {}", buffer.ToString());
        Message message(buffer);
        spdlog::info("Got {}", message.ToString());
        if(message.type == Type::kGameStarted) {
            std::cout << message.boardState.size() << std::endl;
            board_ = message.boardState;
            my_color_ = message.color;
            message.type = Type::kGameStartedAccepted;
            client_->Send(message.ToByteBuffer());
            spdlog::info("Sending {}", message.ToString());
            SetupBoard();
        }else if(message.type == Type::kRequestingMove){
            myTurn_ = true;
            moves_ = message.avaliableMoves;
            board_ = message.boardState;
        }else if(message.type == Type::kMoveOK) {
            moves_ = message.avaliableMoves;
            board_ = message.boardState;
            spdlog::info("Move was correct.");
        }
    }
    
    window_.clear();
    DrawBoard();
    window_.display();
}

void PlayingWindow::SetupBoard() {
    tile_.setOutlineThickness(-5);
    tile_.setOutlineColor(sf::Color::Black);
    tileSize_ = (float)std::min(window_.getSize().x, window_.getSize().y) / board_.size();
    tile_.setSize({tileSize_, tileSize_});
    stone_.setRadius(tileSize_ / 2.5f);
    stone_.setOutlineColor(sf::Color::Black);
    stone_.setOutlineThickness(-5);
    stone_.setOrigin({stone_.getRadius(), stone_.getRadius()});
    shade_.setFillColor(sf::Color(176,176,176));
    shade_.setRadius(stone_.getRadius() / 2.f);
    shade_.setOrigin({shade_.getRadius(), shade_.getRadius()});
}

void PlayingWindow::DisplayMessage(std::string m){
    sf::Text text;
    const auto size = window_.getSize();
    text.setFont(kFont);
    text.setString(m);
    text.setCharacterSize(58);
    text.setFillColor(sf::Color::Red);
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
                textRect.top  + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(size.x/2.0f,size.y/2.0f));
    window_.draw(text);
}

void PlayingWindow::DrawBoard() {
    if(board_.empty()){
        DisplayMessage("Waiting for server...");
        return;
    }
    for(int i = 0; i < board_.size(); i++) {
        for(int j = 0; j < board_.size(); j++) {
            tile_.setPosition(i * tileSize_, j * tileSize_);
            tile_.setFillColor(tile_colors_[(i + j) % 2]);
            stone_.setPosition((i + 0.5f) * tileSize_, (j + 0.5f) * tileSize_);
            window_.draw(tile_);
            if(i == moveSource_.row && j == moveSource_.column)
                stone_.setOutlineColor(sf::Color::Red);
            if(board_[i][j] == Color::kWhite){
                stone_.setFillColor(sf::Color::White);
                window_.draw(stone_);
            }else if(board_[i][j] == Color::kBlack){
                stone_.setFillColor(sf::Color::Black);
                window_.draw(stone_);
            }
            stone_.setOutlineColor(sf::Color::Black);
        }
    }
    for(auto p : shaded_) {
        int r = p.row, c = p.column;
        shade_.setPosition({(r + 0.5f) * tileSize_, (c + 0.5f) * tileSize_});
        window_.draw(shade_);
    }
}

void PlayingWindow::SetClient(TCPClient * c) {
    client_ = c;
}

void PlayingWindow::PoolEvents() {
    sf::Event ev;
    while(window_.pollEvent(ev)) {
        if(ev.type == sf::Event::Closed || ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape){
            window_.close();
            return;
        }
        if(ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
            OnMouseClick();
        }
    }
}

void PlayingWindow::AddToShaded(int r, int c) {
    for(int i = 0; i < shaded_.size(); i++)
        if(shaded_[i].row == r && shaded_[i].column == c)
            return;
    shaded_.push_back(Point(r, c));
}

bool PlayingWindow::CoordsInBoard(int r, int c) {
    return r >= 0 && r < board_.size() && c >= 0 && c < board_.size();
}

bool PlayingWindow::ValidMove(int r, int c) {
    if(!myTurn_)
        return false;
    Point dst(r, c);
    for(auto m : moves_){
        if(moveSource_ == m.GetSource() && m.GetDestination() == dst)
            return true;
    }
    return false;
}

void PlayingWindow::OnMouseClick() {
    auto pos = sf::Mouse::getPosition(window_);
    int r = (int)(pos.x / tileSize_);
    int c = (int)(pos.y / tileSize_);
    spdlog::info("Mouse position: {} {} (board = {} {})", pos.x, pos.y, r, c);
    if(CoordsInBoard(r, c)) {
        if(!shaded_.empty() && ValidMove(r, c) ) {
            moveDestination_ = Point(r, c);
            DoMove();
            return; //TODO ?
        }else {
            shaded_.resize(0);
            if(board_[r][c] == my_color_){
                moveSource_ = Point(r, c);
                for(auto m : moves_)
                    if(m.GetSource() == moveSource_)
                        shaded_.push_back(m.GetDestination());
            }
        }
    }
}

void PlayingWindow::DoMove() {
    if(!myTurn_)
        return;
    Message message;
    message.type = Type::kSendingMove;
    message.moveMade = Move(moveSource_, moveDestination_);
    spdlog::info("Sending {}", message.ToString());
    client_->Send(message.ToByteBuffer());
    myTurn_ = false;
    moveSource_ = Point(-1, -1);
    shaded_.resize(0);
}

bool PlayingWindow::IsOK() const {
    return isOk_;
}


std::string PlayingWindow::GetError() const {
    return error_;
}