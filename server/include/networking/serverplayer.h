#pragma once

#include "common/game/player.h"

class Client;

class ServerPlayer : public Player {
public:

    ServerPlayer(Client * player_client, Color color);

    Color GetColor() const override;
    Move GetMove() const override;
    void Think(Board * board) override;

private:
    Client * client_;
    Color color_;
};