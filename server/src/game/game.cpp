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
        if(players_[1].client->CanReceive()){
            buf = players_[1].client->Receive();
            if(buf.IsEmpty()) {
                status_ = GameStatus::GAME_FORCEFULLY_ENDED;
                InvalidateBothPlayers();
                return false;
            }
            p2_accepted_ = Message(buf).type == Type::kGameStartedAccepted;
        }
    }
    if(p1_accepted_ && p2_accepted_){
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
    CurrentPlayer().client->Send(message.ToByteBuffer());
}

Game::GamePlayer Game::SwitchPlayer() {
    current_player_ = 1 - current_player_;
    return CurrentPlayer();
}

void Game::EndGame(Color result) {
    Message m;
    m.type = Type::kGameEnded;
    m.gameResult = result;
    players_[0].client->Send(m.ToByteBuffer());
    players_[1].client->Send(m.ToByteBuffer());
    ended_ = true;
    status_ = (result == players_[0].color ? GameStatus::P1_WON : GameStatus::P2_WON);
    InvalidateBothPlayers();
}

bool Game::Ended() const {
    return ended_;
}

bool Game::CheckForGameEnd() {
    auto result = board_->GetWinner();
    if(result != Color::kEmpty){
        EndGame(result);
        return true;
    }
    return false;
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
        if(message.type == Type::kSendingMove) {
            AcceptAndDoMove(message);
            if(CheckForGameEnd()) {
                //game ended
                return;
            }
            SwitchPlayer();
            RequestMoveFromCurrentPlayer();
        }
    }
}

void Game::InvalidateBothPlayers() {
    players_[0].client->Invalidate();
    players_[1].client->Invalidate();
}

Game::GameStatus Game::GetStatus() const {
    return status_;
}