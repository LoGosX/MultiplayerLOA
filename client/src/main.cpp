#include <iostream>

#include "common/game/color.h"
#include "spdlog/spdlog.h"

int main(int argc, char const *argv[])
{
    std::cout << "Hello World" << std::endl;
    Color c = Color::kBlack;
    std::cout << static_cast<int>(c) << std::endl;
    spdlog::info("Welcome to spdlog from client!");
    return 0;
}
