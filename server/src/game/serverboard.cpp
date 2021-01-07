#include "game/serverboard.h"

ServerBoard::ServerBoard(int s) :
    kSize(s) {
        CreateBoard();
    }

void ServerBoard::CreateBoard() {
    board_ = std::vector<std::vector<Color>>(kSize, std::vector<Color>(kSize, Color::kEmpty));
    for(int i = 1; i < kSize - 1; i++) {
        board_[0][i] = board_[kSize - 1][i] = Color::kBlack;
        board_[i][0] = board_[i][kSize - 1] = Color::kWhite;
    }
}

void ServerBoard::DoMove(Move m) {
    Point src = m.GetSource(), dst = m.GetDestination();
    board_[dst.row][dst.column] = board_[src.row][src.column];
    board_[src.row][src.column] = Color::kEmpty;
}

void ServerBoard::WriteToBuffer(ByteBuffer & buffer) {
    buffer.WriteByte(static_cast<char>(kSize));
    for(int r = 0; r < kSize; r++) {
        for(int c = 0; c < kSize; c++) {
            auto color = GetColorAt({r, c});
            buffer.WriteByte(static_cast<char>(color));
        }
    }
}

Color ServerBoard::GetColorAt(Point p) const {
    return board_[p.row][p.column];
}

int CountPiecesAlong(const std::vector<std::vector<Color>> & board, int r, int c, int dr, int dc) {
    int count = 0;
    for(int i = -1; i <= 1; i += 2) {
        int cr = r, cc = c;
        while(cr >= 0 && cr < board.size() && cc >= 0 && cc < board.size()) {
            if(board[cr][cc] != Color::kEmpty)
                count++;
            cr += i * dr;
            cc += i * dc;
        }
    } 
    if(board[r][c] != Color::kEmpty)
        count--;
    return count;
}

bool CanPlaceInDirection(const std::vector<std::vector<Color>> & board, int src_r, int src_c, Color c, int dr, int dc, int dst) {
    while(src_r > 0 && src_r + 1 < board.size() && src_c > 0 && src_c + 1 < board.size() && dst > 0 && board[src_r][src_c] != c) {
        src_r += dr;
        src_c += dc;
        dst--;
    }
    if(!(src_r >= 0 && src_r < board.size() && src_c >= 0 && src_c < board.size()))
        return false;
    return dst == 0 && board[src_r][src_c] != c;
}

std::vector<Move> ServerBoard::GetMovesFor(Color color) const {
    std::vector<Move> moves;
    for(int r = 0; r < kSize; r++) {
        for(int c = 0; c < kSize; c++) {
            if(GetColorAt({r, c}) == color) {
                for(int dr = -1; dr <= 1; dr++){
                    for(int dc = -1; dc <= 1; dc++){
                        if(dr == 0 && dc == 0)
                            continue;
                        int dst = CountPiecesAlong(board_, r, c, dr, dc);
                        if(CanPlaceInDirection(board_, r, c, color, dr, dc, dst)) {
                            moves.push_back(Move({r, c}, {r + dst * dr, c + dst * dc}));
                        }
                    }
                }
            } 
        }
    }   
    return moves;
}

int ServerBoard::GetSize() const {
    return kSize;
}

std::string ServerBoard::ToString() const {
    std::string s;
    for(int r = 0; r < kSize; r++) {
        for(int c = 0; c < kSize; c++){
            if(GetColorAt({r, c}) == Color::kEmpty)
                s += ' ';
            else
                s += static_cast<int>(GetColorAt({r, c}));
        }
        s += '\n';
    }
    return s;
}