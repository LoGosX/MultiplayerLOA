#include "networking/message.h"
#include "spdlog/spdlog.h"
#include <sstream>

Message::Message(ByteBuffer & buffer) {
    FromByteBuffer(buffer);
}

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

ByteBuffer Message::ToByteBuffer() const {
    ByteBuffer buffer;
    buffer.WriteByte(static_cast<char>(type));
    if(type == Type::kSendingName) {
        buffer.WriteString(name);
    }else if(type == Type::kRequestListOfPlayers){

    }else if(type == Type::kGameMove) {
        buffer.WriteByte(move.GetSource().row);
        buffer.WriteByte(move.GetSource().column);
        buffer.WriteByte(move.GetDestination().row);
        buffer.WriteByte(move.GetDestination().column);
    }else if(type == Type::kMessage) {
        buffer.WriteString(message);
    }else if(type == Type::kBoard) {
        WriteBoardToBuffer(buffer, boardSize, board);
    }else if(type == Type::kSearchingForGame){
        buffer.WriteString(name + '\n' + opponentName);
    }else if(type == Type::kGameStart){
        
    }else{
        spdlog::error("Unknown message Type!\n");
    }
    return buffer;
}

std::vector<std::string> SplitString(const std::string& str)
{
    auto result = std::vector<std::string>{};
    auto ss = std::stringstream{str};
    for (std::string line; std::getline(ss, line, '\n');)
        result.push_back(line);
    return result;
}

void Message::FromByteBuffer(ByteBuffer & buffer) {
    Type t = static_cast<Type>(buffer.ReadByte());
    if(t == Type::kSendingName) {
        name = buffer.ReadString();
    }else if(t == Type::kRequestListOfPlayers) {
        names = SplitString(buffer.ReadString());
    }else if(t == Type::kGameMove) {
        int r = buffer.ReadByte();
        int c = buffer.ReadByte();
        int dstr = buffer.ReadByte();
        int dstc = buffer.ReadByte();
        move = Move(Point(r, c), Point(dstr, dstc));
    }else if(t == Type::kMessage) {
        message = buffer.ReadString();
    }else if(t == Type::kBoard){
        auto p = ReadBoardFromBuffer(buffer);
        boardSize = p.first;
        board = p.second;
    } else if(t == Type::kSearchingForGame){
        auto names = SplitString(buffer.ReadString());
        if(names.size() != 2) {
            spdlog::error("Wrong number of received names! Got {} instead of 2", names.size());
        }else{
            name = names[0];
            opponentName = names[1];
        }
    }
    else{
        spdlog::error("Unknown message type when reading!\n");
        return;
    }
    type = t;
}

std::string Message::ToString() const {
    std::string result = std::to_string(static_cast<int>(type)) + "-> ";
    if(type == Type::kSendingName) {
        result += "NAME: " + name;
    }else if(type == Type::kRequestListOfPlayers) {
        result += "NAMES:"; 
        for(auto & name : names)
            result += ' ' + name;
    } else if (type == Type::kGameMove) {
        result += "MOVE (" + std::to_string(move.GetSource().row) + ',' +
            std::to_string(move.GetSource().column) + ") -> (" +
            std::to_string(move.GetDestination().row) + ',' +
            std::to_string(move.GetDestination().column) + ')'; 
    }else if(type == Type::kMessage) {
        result += "MESSAGE: " + message;
    }else if(type == Type::kBoard) {
        result += "BOARD SIZE: " + std::to_string(boardSize);
    }else if(type == Type::kSearchingForGame){
        result += name + " wants to play against " + opponentName;
    }else{
        spdlog::error("UNKNOWN MESSAGE TYPE {}", static_cast<int>(type));
    }
    return result;
}