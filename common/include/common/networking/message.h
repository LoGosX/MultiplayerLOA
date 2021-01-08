#pragma once

#include "common/game/move.h"
#include "common/utils/bytebuffer.h"
#include "common/game/color.h"

enum class Type : char {
    kSendingName = 0, // only name is set
    kRequestListOfPlayers = 1, //nothing is set
    kGameMove = 2, //only move is set
    kMessage = 3, //only message is set
    kBoard = 4, //boardSize & board,
    kSearchingForGame = 5 //name & opponentName
};

struct Message {

    Message() = default;
    Message(ByteBuffer & bytebuffer);

    Type type;
    std::string name;
    std::string opponentName;
    std::vector<std::string> names;
    Move move;
    std::string message;
    int boardSize;
    std::vector<std::vector<Color>> board;
    ByteBuffer ToByteBuffer() const;
    void FromByteBuffer(ByteBuffer & buffer);

    std::string ToString() const;
};