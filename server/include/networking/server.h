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
    void RemoveClient(int cfd);
    void SetTimeout(int seconds);
private:
    void UpdateFDMax();
    fd_set rmask_, wmask_;
    int sfd_, fdmax_, timeout_;
    std::unordered_map<int, std::unique_ptr<ServerClient>> clients_;
};