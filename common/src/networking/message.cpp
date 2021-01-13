#include "networking/message.h"
#include "spdlog/spdlog.h"
#include <sstream>

void WriteBoardToBuffer(ByteBuffer & buf, const int kBoardSize, const std::vector<std::vector<Color>> & board) {
    buf.WriteByte(kBoardSize);
    for(int r = 0; r < kBoardSize; r++) {
        for(int c = 0; c < kBoardSize; c++) {
            buf.WriteByte(static_cast<char>(board[r][c]));
        }
    }
}

std::pair<int, std::vector<std::vector<Color>>> ReadBoardFromBuffer(ByteBuffer & buffer) {
    int boardSize = buffer.ReadByte();
    std::vector<std::vector<Color>> board;
    board.resize(boardSize);
    for(int r = 0; r < boardSize; r++){
        board[r].resize(boardSize);
        for(int c = 0; c < boardSize; c++)
            board[r][c] = static_cast<Color>(buffer.ReadByte());
    }
    return {boardSize, board};
}

void WriteMoveToBuffer(ByteBuffer & buffer, Move move) {
    buffer.WriteByte((char)move.GetSource().row);
    buffer.WriteByte((char)move.GetSource().column);
    buffer.WriteByte((char)move.GetDestination().row);
    buffer.WriteByte((char)move.GetDestination().column);
}

std::vector<std::string> SplitString(const std::string& str)
{
    auto result = std::vector<std::string>{};
    auto ss = std::stringstream{str};
    for (std::string line; std::getline(ss, line, '\n');)
        result.push_back(line);
    return result;
}

Move ReadMoveFromBuffer(ByteBuffer & buffer) {
    int r = buffer.ReadByte();
    int c = buffer.ReadByte();
    int dstr = buffer.ReadByte();
    int dstc = buffer.ReadByte();
    return Move(Point(r, c), Point(dstr, dstc));
}

Message::Message(ByteBuffer & buffer) {
    FromByteBuffer(buffer);
}

Message::Message(ByteBuffer && buffer) {
    FromByteBuffer(buffer);
}

ByteBuffer Message::ToByteBuffer() const {
    ByteBuffer buffer;
    buffer.WriteByte(static_cast<char>(type));
    
    if(type == Type::kGameStarted) {
        buffer.WriteByte(static_cast<char>(color));
        WriteBoardToBuffer(buffer, boardSize, boardState);
    } else if(type == Type::kGameStartedAccepted) {
        
    } else if(type == Type::kRequestingMove) {
        buffer.WriteByte(avaliableMovesCount);
        for(auto m : avaliableMoves)
            WriteMoveToBuffer(buffer, m);
        WriteBoardToBuffer(buffer, boardSize, boardState);
    } else if(type == Type::kSendingMove) {
        WriteMoveToBuffer(buffer, moveMade);
    } else if(type == Type::kMoveOK) {
        WriteBoardToBuffer(buffer, boardSize, boardState);
    } else if(type == Type::kSearchingForGame){
        buffer.WriteString(opponentIP);
    }else {
        spdlog::error("Unknown message type received! {}\n", static_cast<int>(type));
    }
    buffer.PadToBufferSize();
    return buffer;
}



void Message::FromByteBuffer(ByteBuffer & buffer) {
    type = static_cast<Type>(buffer.ReadByte());
    if(type == Type::kGameStarted) {
        color = static_cast<Color>(buffer.ReadByte());
        auto p = ReadBoardFromBuffer(buffer);
        boardSize = p.first;
        boardState = p.second;
    } else if(type == Type::kGameStartedAccepted) {

    } else if(type == Type::kRequestingMove) {
        avaliableMovesCount = buffer.ReadByte();
        avaliableMoves.resize(avaliableMovesCount);
        for(int i = 0; i < avaliableMovesCount; i++)
            avaliableMoves[i] = ReadMoveFromBuffer(buffer);
        auto p = ReadBoardFromBuffer(buffer);
        boardSize = p.first;
        boardState = p.second;
    } else if(type == Type::kSendingMove) {
        moveMade = ReadMoveFromBuffer(buffer);
    } else if(type == Type::kMoveOK) {
        auto p = ReadBoardFromBuffer(buffer);
        boardSize = p.first;
        boardState = p.second;
    } else if(type == Type::kSearchingForGame){
        opponentIP = buffer.ReadString();
    }else {
        spdlog::error("Unknown message type! {}\n", static_cast<int>(type));
    }
}

std::string Message::ToString() const {
    std::stringstream ss;
    ss << "Message(";
    if(type == Type::kGameStarted) {
        ss << "kGameStarted";
    }else if(type == Type::kGameStartedAccepted) {
        ss << "kGameStartedAccepted";
    }else if(type == Type::kRequestingMove) {
        ss << "kRequestingMove,nMoves=" << avaliableMoves.size();
    }else if(type == Type::kSendingMove) {
        ss << "kSendingMove,move=(" << moveMade.GetSource().row << ',' << moveMade.GetSource().column
            << ")->(" << moveMade.GetDestination().row << ',' << moveMade.GetDestination().column << ')';
    }else if(type == Type::kMoveOK) {
        ss << "kMoveOK";
    }else if(type == Type::kSearchingForGame) {
        ss << "kSearchingForGame,opponentIp=" << opponentIP;
    }else {
        ss << "type=" << static_cast<int>(type);
    }
    ss << ')';
    return ss.str();
}