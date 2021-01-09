#pragma once
#include <memory>
#include <unordered_map>
#include <unistd.h>
#include <fcntl.h>


#include "networking/serverclient.h" 
#include "common/game/game.h"


class Server {

public:
    void Update();
    void Prepare();
    void SetTimeout(int seconds);
private:

    struct GameSearchInfo {
        ServerClient * who;
        std::string name;
        std::string targetName;
    };

    void UpdateFDMax();
    void ParseMessage(ServerClient * client);
    void DeleteClients();
    void MarkClientToDelete(int cfd);
    void TryToStartGame();
    
    fd_set rmask_, wmask_;
    int sfd_, fdmax_, timeout_;
    std::unordered_map<int, std::unique_ptr<ServerClient>> clients_;
    std::vector<int> clients_to_delete_;
    std::vector<GameSearchInfo> clients_looking_for_game_;
    std::vector<std::unique_ptr<Game>> games_;    
};