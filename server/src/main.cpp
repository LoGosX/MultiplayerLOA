#include <iostream>

#include "spdlog/spdlog.h"
#include "networking/server.h"

void testBoard();

int main(int argc, char const *argv[])
{
    spdlog::info("sizeof(char)={}",sizeof(char));
    //testBoard();
    Server server;
    server.Prepare();
    server.SetTimeout(5);
    while(true) {
        server.Update();
    }
    return 0;
}



#include "game/serverboard.h"
void testBoard() {
    ServerBoard board;
    while(true) {
        std::cout << board.ToString() << std::endl;
        auto mWhite = board.GetMovesFor(Color::kWhite);
        auto mBlack = board.GetMovesFor(Color::kBlack);
        std::cout << "#" << mWhite.size() << " moves avaliable for white\n";
        std::cout << "#" << mBlack.size() << " moves avaliable for black\n";
        std::cin.get();
    }
}