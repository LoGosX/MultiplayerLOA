#pragma once
#include "app/window.h"
#include "common/game/color.h"
#include "common/game/move.h"
#include "common/game/point.h"

#include <vector>

class TCPClient;

class PlayingWindow : public Window {
public:
    using Window::Window;
    void Update() override;
    void PoolEvents() override;
    void SetClient(TCPClient *);
    bool IsOK() const;
    std::string GetError() const;
private:
    void DrawBoard();
    void SetupBoard();
    void OnMouseClick();
    bool CoordsInBoard(int r, int c);
    void DoMove();
    bool ValidMove(int r, int c);
    void AddToShaded(int r, int c);
    void DisplayMessage(std::string);
    std::vector<std::vector<Color>> board_;
    std::vector<Move> moves_;
    std::vector<Point> shaded_;
    Color my_color_;
    TCPClient * client_;
    sf::RectangleShape tile_;
    sf::CircleShape stone_;
    sf::CircleShape shade_;
    sf::Color tile_colors_[2] = {sf::Color::Green, sf::Color::Yellow};
    Point moveDestination_;
    Point moveSource_;
    float tileSize_;
    bool myTurn_ = false;
    bool isOk_;
    std::string error_;
};