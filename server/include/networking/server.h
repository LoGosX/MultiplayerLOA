#pragma once
#include <memory>
#include <unordered_map>
#include <unistd.h>
#include <fcntl.h>


#include "networking/serverclient.h" 
#include "game/game.h"


class Server {
public:
    void Update();
    void Prepare();
    void SetTimeout(int seconds);
private:
    void UpdateFDMax();
    void ParseMessage(ServerClient * client);
    void DeleteClients();
    void MarkClientToDelete(int cfd);
    void TryToStartGame();
    void RemoveInvalidated();
    std::vector<int> clients_to_delete_;
    fd_set rmask_, wmask_;
    int sfd_, fdmax_, timeout_;

    struct ConnectionInfo {
        ServerClient * client;
        std::string ip, opponentIP;
        bool inGame = false;
    };
    std::vector<std::unique_ptr<ServerClient>> clients_;
    std::vector<std::unique_ptr<Game>> games_;
    std::vector<ConnectionInfo> infos_;
};