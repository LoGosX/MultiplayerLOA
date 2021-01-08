#pragma once
#include <memory>
#include <unordered_map>
#include <unistd.h>
#include <fcntl.h>


#include "networking/serverclient.h" 

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
    
    fd_set rmask_, wmask_;
    int sfd_, fdmax_, timeout_;
    std::unordered_map<int, std::unique_ptr<ServerClient>> clients_;
    std::vector<int> clients_to_delete_;

    std::vector<std::pair<ServerClient *, std::string>> clients_looking_for_game_;
};