#include <iostream>

#include "spdlog/spdlog.h"
#include "networking/server.h"

int main(int argc, char const *argv[])
{
    Server server;
    server.Prepare();
    server.SetTimeout(5);
    while(true) {
        server.Update();
    }
    return 0;
}
