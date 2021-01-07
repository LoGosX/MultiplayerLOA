#include "game/game.h"
#include "game/player.h"
#include "game/board.h"

Game::Game(Player * white, Player * black, Board * board) : 
    white_(white), black_(black), board_(board) {}

Color Game::GetCurrentColor() const {
    return current_color_;
}

Player * Game::GetCurrentPlayer() const {
    return GetCurrentColor() == Color::kWhite ? white_ : black_;
}

void Game::SwitchPlayers() {
    if(current_color_ == Color::kWhite)
        current_color_ = Color::kBlack;
    else
        current_color_ = Color::kWhite;
}

bool Game::Update() {
    Player * current_player = GetCurrentPlayer();
    if(current_player->IsReady()) {
        Move move = current_player->GetMove();

        board_->DoMove(move);

        current_player->SetReady(false);
        SwitchPlayers();
        current_player = GetCurrentPlayer();
        current_player->Think(board_);
        turn_count_++;
        return true;
    }

    return false;
}

