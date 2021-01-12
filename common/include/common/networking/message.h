#pragma once

#include "common/game/move.h"
#include "common/utils/bytebuffer.h"
#include "common/game/color.h"

#include <string>

enum class Type : char {
    kGameStarted = 0, //waits for answer from both players
    kGameStartedAccepted = 1, //answer from clients after game starts
    kRequestingMove = 2, //server requests move from the player
    kSendingMove = 3, //client sends move made
    kMoveOK = 4, //move sent is valid move
    kSearchingForGame = 5, //first message from the client containing enemy ip
};

struct Message {

    Message() = default;
    Message(ByteBuffer &);
    Message(ByteBuffer &&);

    Type type;

    //FIELDS

    //kGameStarted
    Color color; //color of player that this message is sent to
    int boardSize;
    std::vector<std::vector<Color>> boardState;

    //kGameStartedAccepted
    std::string opponentIP;

    //kRequestingMove
    int avaliableMovesCount;
    std::vector<Move> avaliableMoves;
    bool firstMoveInGame;
    Move lastOpponentMove;

    //kSendingMove
    Move moveMade;

    //kMoveOK

    ByteBuffer ToByteBuffer() const;
    void FromByteBuffer(ByteBuffer &);
    std::string ToString() const;
};