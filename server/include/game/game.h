#pragma once

#include "common/game/color.h"
#include "common/networking/message.h"

class ServerClient;
class ServerBoard;

class Game {

public:
    enum class GameStatus {
        P1_TURN,
        P2_TURN,
        P1_WON,
        P2_WON,
        GAME_FORCEFULLY_ENDED
    };

    Game(ServerClient * p1, ServerClient * p2, ServerBoard * board);

    void Update();

    void Start();

    GameStatus GetStatus() const;
private:
    bool CheckForAccept();
    void AcceptAndDoMove(Message);
    void RequestMoveFromCurrentPlayer();
    void InvalidateBothPlayers();
    bool p1_accepted_ = false, p2_accepted_ = false;
    GameStatus status_;
    struct GamePlayer
    {
        ServerClient * client;
        Color color;
    };
    GamePlayer CurrentPlayer() const;
    GamePlayer SwitchPlayer();
    GamePlayer players_[2];
    int current_player_ = 0;
    ServerBoard * board_ = nullptr;
};