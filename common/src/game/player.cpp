#include "game/player.h"

bool Player::IsReady() const {
    return ready_;
}

void Player::SetReady(bool v) {
    ready_ = v;
}