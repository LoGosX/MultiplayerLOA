#include "networking/server.h"
#include "common/networking/message.h"
#include "game/serverboard.h"

#include "spdlog/spdlog.h"

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h> //sleep

void Server::Prepare() {
    socklen_t slt;
    int on = 1;
    sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(1234);
    sfd_ = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(sfd_, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
    bind(sfd_, (sockaddr *)&saddr, sizeof(saddr));
    listen(sfd_, 5);
    fdmax_ = sfd_;
    FD_ZERO(&rmask_);
    FD_ZERO(&wmask_);
}

void Server::UpdateFDMax() {
    while (fdmax_ > sfd_ && !(FD_ISSET(fdmax_, &rmask_) || FD_ISSET(fdmax_, &wmask_)))
        fdmax_ -= 1;
}

void Server::SetTimeout(int t) {
    timeout_ = t;
}

void Server::Update() {
    fd_set rmask_tmp, wmask_tmp;
    socklen_t slt;
    int cfd;
    wmask_tmp = wmask_;
    rmask_tmp = rmask_;
    FD_SET(sfd_, &rmask_tmp);
    timeval timeout;
    timeout.tv_sec = timeout_;
    timeout.tv_usec = 0;
    int rc = select(fdmax_ + 1, &rmask_tmp, &wmask_tmp, (fd_set *)0, &timeout);
    if (rc == 0)
    {
        spdlog::info("timed out\n");
        return;
    }
    sockaddr_in caddr;
    if (FD_ISSET(sfd_, &rmask_tmp))
    {
        slt = sizeof(caddr);
        cfd = accept(sfd_, (sockaddr *)&caddr, &slt);
        spdlog::info("New connection: {}\n", inet_ntoa((in_addr)caddr.sin_addr));
        FD_SET(cfd, &rmask_);
        FD_SET(cfd, &wmask_);
        clients_.emplace_back(std::make_unique<ServerClient>(cfd));
        if (cfd > fdmax_)
            fdmax_ = cfd;
    }
    else
    {
        for(auto & client : clients_)
        {
            int cfd = client->GetFd();
            if (FD_ISSET(cfd, &rmask_tmp))
            {
                client->SetCanReceive(true);
            }else{
                client->SetCanReceive(false);
            }
            if (FD_ISSET(cfd, &wmask_tmp))
            {
                client->SetCanSend(true);
            }else {
                client->SetCanSend(false);
            }
        }
    }
    
    spdlog::info("TryToStartGame()\n");
    TryToStartGame();

    spdlog::info("Updating games\n");
    for(auto & game : games_){
        game->Update();
    }

    DeleteClients();

    sleep(1);
}

void Server::DeleteClients() {
    //todo delete clients
    UpdateFDMax();
    clients_to_delete_.resize(0);
}

void Server::TryToStartGame() {
    if(clients_.size() == 2 && games_.size() == 0) {
        spdlog::info("???\n");
        spdlog::info("{} {}\n", clients_.size(), games_.size());
        spdlog::info("Starting new game for cfds {} and {}", clients_[0]->GetFd(), clients_[1]->GetFd());
        spdlog::info("??? 2\n");
        games_.emplace_back(
            std::make_unique<Game>(clients_[0].get(), clients_[1].get(), new ServerBoard())
        );
        spdlog::info("??? 3\n");
    }
}