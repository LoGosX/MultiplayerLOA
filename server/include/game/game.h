#pragma once

#include "common/game/color.h"
#include "common/networking/message.h"

class Client;
class ServerBoard;

class Game {

public:
    enum class GameStatus {
        P1_TURN,
        P2_TURN,
        P1_WON,
        P2_WON
    };

    Game(Client * p1, Client * p2, ServerBoard * board);

    void Update();

    void Start();

    GameStatus GetStatus() const;

private:
    bool CheckForAccept();
    void AcceptAndDoMove(Message);
    void RequestMoveFromCurrentPlayer();
    bool p1_accepted_ = false, p2_accepted_ = false;
    GameStatus status_;
    struct GamePlayer
    {
        Client * client;
        Color color;
    };
    GamePlayer CurrentPlayer() const;
    GamePlayer SwitchPlayer();
    GamePlayer players_[2];
    int current_player_ = 0;
    ServerBoard * board_ = nullptr;
};