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
    Color GetColorAt(int r, int c) const override;
    void WriteToBuffer(ByteBuffer & buffer);

    std::string ToString() const;

    std::vector<std::vector<Color>> GetRawBoard() const;
private:
    bool ValidCoords(int r, int c) const; 
    void CreateBoard();
    bool CanPlaceInDirection(int r, int c, Color color, int dr, int dc, int dst) const;
    const int kSize;
    std::vector<std::vector<Color>> board_;
};