#pragma once

#include <string>

#include "common/game/board.h"
#include "common/utils/bytebuffer.h"

class ServerBoard : public Board {
public:

    ServerBoard(int size=8);

    std::vector<Move> GetMovesFor(Color color) const override;
    void DoMove(Move move) override;
    int GetSize() const override;
    Color GetColorAt(Point p) const override;
    void WriteToBuffer(ByteBuffer & buffer);

    std::string ToString() const;

    std::vector<std::vector<Color>> GetRawBoard() const;
private:
    void CreateBoard();
    const int kSize;
    std::vector<std::vector<Color>> board_;
};