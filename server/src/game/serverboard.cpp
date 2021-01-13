#include "game/serverboard.h"

#include "spdlog/spdlog.h"
#include <iostream>
#include <queue>

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
            auto color = GetColorAt(r, c);
            buffer.WriteByte(static_cast<char>(color));
        }
    }
}

Color ServerBoard::GetColorAt(int r, int c) const {
    if(r < 0 || r >= kSize || c < 0 || c >= kSize) {
        spdlog::error("{} {} are not valid board coordinates for board of size {}!", r, c, kSize);
        return Color::kEmpty;
    }
    return board_[r][c];
}

int CountPiecesAlong(const std::vector<std::vector<Color>> & board, int r, int c, int dr, int dc) {
    int count = 0;
    for(int i = -1; i <= 1; i += 2) {
        int cr = r, cc = c;
        while(cr >= 0 && cr < (int)board.size() && cc >= 0 && cc < (int)board.size()) {
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

bool ServerBoard::CanPlaceInDirection(int r, int c, Color color, int dr, int dc, int dst) const {
    while(ValidCoords(r, c) && dst > 0 && (board_[r][c] == color || board_[r][c] == Color::kEmpty)) {
        r += dr;
        c += dc;
        dst--;
    }
    return ValidCoords(r, c) && dst == 0 && board_[r][c] != color;
}

bool ServerBoard::ValidCoords(int r, int c) const {
    return r >= 0 && r < kSize && c >= 0 && c < kSize;
}

std::vector<Move> ServerBoard::GetMovesFor(Color color) const {
    std::vector<Move> moves;
    for(int r = 0; r < kSize; r++) {
        for(int c = 0; c < kSize; c++) {
            if(GetColorAt(r, c) == color) {
                for(int dr = -1; dr <= 1; dr++){
                    for(int dc = -1; dc <= 1; dc++){
                        if(dr == 0 && dc == 0)
                            continue;
                        int dst = CountPiecesAlong(board_, r, c, dr, dc);
                        if(CanPlaceInDirection(r, c, color, dr, dc, dst)) {
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
            if(GetColorAt(r, c) == Color::kEmpty)
                s += ' ';
            else
                s += '0' + static_cast<int>(GetColorAt(r, c));
        }
        s += '\n';
    }
    return s;
}

std::vector<std::vector<Color>> ServerBoard::GetRawBoard() const {
    return board_;
}

void ServerBoard::FillConnected(std::vector<std::vector<bool>> & visited, int r, int c, Color color) const {
    std::queue<std::pair<int, int>> q;
    q.emplace(r, c);
    visited[r][c] = true;
    while (!q.empty())
    {
        auto [cr, cc] = q.front();
        q.pop();
        for(int dr = -1; dr <= 1; dr++){
            for(int dc = -1; dc <= 1; dc++) {
                if(dr == 0 && dc == 0)
                    continue;
                int nr = cr + dr, nc = cc + dc;
                if(ValidCoords(nr, nc) && !visited[nr][nc] && board_[nr][nc] == color){
                    visited[nr][nc] = true;
                    q.emplace(nr, nc);
                }
            }
        }
    }
}


int ServerBoard::GetNumberOfConnectedRegions(Color c) const {
    std::vector<std::vector<bool>> visited(board_.size());
    for(auto & v : visited){
        v.resize(board_.size(), false);
    }
    int nRegions = 0;
    for(int i = 0; i < (int)board_.size(); i++) {
        for(int j = 0; j < (int)board_.size(); j++) {
            if(board_[i][j] == c && !visited[i][j]){
                nRegions++;
                FillConnected(visited, i, j, c);
            }
        }
    }
    return nRegions;
}

Color ServerBoard::GetWinner() const {
    int nRegP1 = GetNumberOfConnectedRegions(Color::kBlack);
    int nRegP2 = GetNumberOfConnectedRegions(Color::kWhite);
    if(nRegP1 == 1)
        return Color::kBlack;
    if(nRegP2 == 1)
        return Color::kWhite;
    return Color::kEmpty;
}