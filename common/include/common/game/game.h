#pragma once

#include "game/color.h"

//forward declarations
class Player;
class Board;

class Game {
public:

    Game(Player * white_player, Player * black_player, Board * board);
    Game(const Game&) = delete;

    bool Update(); //updates state of the game

private:
    Player * const white_;
    Player * const black_;
    Board * const board_;

    int turn_count_ = 0;

    Color current_color_ = Color::kWhite;
    Color GetCurrentColor() const;
    Player * GetCurrentPlayer() const;
    void SwitchPlayers();
};