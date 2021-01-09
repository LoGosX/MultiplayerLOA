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
    kSearchingForGame = 5, //name & opponentName
    kGameStart = 6 //gameAccepted & board & yourColor & yourTurn & avaliableMoves (if yourTurn)
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
    bool gameAccepted;
    Color yourColor;
    bool yourTurn;
    std::vector<Move> avaliableMoves;
    std::vector<std::vector<Color>> board;
    ByteBuffer ToByteBuffer() const;
    void FromByteBuffer(ByteBuffer & buffer);

    std::string ToString() const;
};