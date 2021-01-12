#include "game/game.h"
#include "game/serverboard.h"
#include "networking/serverclient.h"
#include "common/networking/message.h"

#include "spdlog/spdlog.h"

#include <iostream>

Game::Game(ServerClient * p1, ServerClient * p2, ServerBoard * board) {
    players_[0].client = p1;
    players_[0].color = Color::kBlack;
    players_[1].client = p2;
    players_[1].color = Color::kWhite;
    board_ = board;

    Start();
}


void Game::Start() {
    spdlog::info("Starting game\n");
    Message message;
    message.type = Type::kGameStarted;
    message.boardSize = board_->GetSize();
    message.boardState = board_->GetRawBoard();
    
    message.color = players_[0].color;
    players_[0].client->Send(message.ToByteBuffer());

    message.color = players_[1].color;
    players_[1].client->Send(message.ToByteBuffer());

    status_ = GameStatus::P1_TURN;
    message.boardState = board_->GetRawBoard();
    spdlog::info("Game started\n");
}

bool Game::CheckForAccept() {
    if(p1_accepted_ && p2_accepted_)
        return true;
    ByteBuffer buf;
    if(!p1_accepted_) {
        if(players_[0].client->CanReceive()) {
            buf = players_[0].client->Receive();
            if(buf.IsEmpty()) {
                status_ = GameStatus::GAME_FORCEFULLY_ENDED;
                InvalidateBothPlayers();
                return false;
            }
            p1_accepted_ = Message(buf).type == Type::kGameStartedAccepted;
        }
    }
    if(!p2_accepted_) {
        buf = players_[1].client->Receive();
        if(buf.IsEmpty()) {
            status_ = GameStatus::GAME_FORCEFULLY_ENDED;
            InvalidateBothPlayers();
            return false;
        }
        p2_accepted_ = Message(buf).type == Type::kGameStartedAccepted;
    }
    if(p1_accepted_ && p2_accepted_){
        spdlog::info("Both players accepted\n");
        return true;
    }else{
        return false;
    }
}


void Game::AcceptAndDoMove(Message message) {
    board_->DoMove(message.moveMade);
    message.type = Type::kMoveOK;
    message.boardSize = board_->GetSize();
    message.boardState = board_->GetRawBoard();
    players_[current_player_].client->Send(message.ToByteBuffer());
}

Game::GamePlayer Game::CurrentPlayer() const {
    return players_[current_player_];
}

void Game::RequestMoveFromCurrentPlayer() {
    Message message;
    message.type = Type::kRequestingMove;
    auto moves = board_->GetMovesFor(CurrentPlayer().color);
    message.avaliableMovesCount = moves.size();
    message.avaliableMoves = moves;
    message.boardSize = board_->GetSize();
    message.boardState = board_->GetRawBoard();
    spdlog::info("Sending {}", message.ToString());
    CurrentPlayer().client->Send(message.ToByteBuffer());
}

Game::GamePlayer Game::SwitchPlayer() {
    current_player_ = 1 - current_player_;
    return CurrentPlayer();
}

void Game::Update() {
    if(status_ == GameStatus::GAME_FORCEFULLY_ENDED)
        return;
    if(!(p1_accepted_ && p2_accepted_)) {
        if(CheckForAccept()) {
            RequestMoveFromCurrentPlayer();
        }
    }
    auto [client, color] = CurrentPlayer();
    if(client->CanReceive()) {
        ByteBuffer buf = client->Receive();
        if(buf.IsEmpty()){
            status_ = GameStatus::GAME_FORCEFULLY_ENDED;
            InvalidateBothPlayers();
            return;
        }
        Message message(buf);
        spdlog::info("Got {}", message.ToString());
        if(message.type == Type::kSendingMove) {
            AcceptAndDoMove(message);
            SwitchPlayer();
            RequestMoveFromCurrentPlayer();
            spdlog::info("Move was made\n");
        }

        std::cout << board_->ToString() << "\n---------\n"; 
    }
}

void Game::InvalidateBothPlayers() {
    players_[0].client->Invalidate();
    players_[1].client->Invalidate();
}

Game::GameStatus Game::GetStatus() const {
    return status_;
}