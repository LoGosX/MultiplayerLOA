#include "game/serverplayer.h"
#include "common/networking/client.h"

ServerPlayer::ServerPlayer(Client * c) : 
    client_(c) {}

Color ServerPlayer::GetColor() const {
    return color_;
}

Move ReadMoveFromBuffer(ByteBuffer & buffer) {
    int src_r = buffer.ReadByte();
    int src_c = buffer.ReadByte();
    int dst_r = buffer.ReadByte();
    int dst_c = buffer.ReadByte();
    return Move(Point(src_r, src_c), Point(dst_r, dst_c));
}

Move ServerPlayer::GetMove() {
    auto buffer = player_client->Receive();
    return ReadMoveFromBuffer(buffer);
}

void ServerPlayer::Think(Board * board) {
    ByteBuffer buffer;
    board->WriteToBuffer(buffer);
    client_->Send(buffer);
}

bool ServerPlayer::IsReady() {
    if(CanReceive()) {

    }else{
        return false;
    }
}