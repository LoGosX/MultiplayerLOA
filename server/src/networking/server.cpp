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
    spdlog::info("Update");
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
        std::string ip(inet_ntoa((in_addr)caddr.sin_addr));
        spdlog::info("New connection: {}\n", ip);
        FD_SET(cfd, &rmask_);
        FD_SET(cfd, &wmask_);
        clients_.emplace_back(std::make_unique<ServerClient>(cfd));
        infos_.push_back({clients_.back().get(), ip, ""});
        if (cfd > fdmax_)
            fdmax_ = cfd;
    }
    else
    {
        RemoveInvalidated();
        for(auto & client : clients_)
        {
            if(!client->IsValid())
                continue;
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
    
    TryToStartGame();

    for(auto & game : games_){
        game->Update();
    }

    DeleteClients();

    sleep(1);
}

void Server::RemoveInvalidated() {
    for(int i = infos_.size() - 1; i >= 0; i--) {
        if(!infos_[i].client->IsValid()){
            spdlog::info("{} invalidated", infos_[i].ip);
            std::swap(infos_[i], infos_.back());
            infos_.pop_back();
        }
    }
    for(int i = clients_.size() - 1; i >= 0; i--) {
        if(!clients_[i]->IsValid()){
            std::swap(clients_[i], clients_.back());
            clients_.pop_back();
        }
    }
    for(int i = games_.size() - 1; i >= 0; i--) {
        if(games_[i]->GetStatus() == Game::GameStatus::GAME_FORCEFULLY_ENDED) {
            std::swap(games_[i], games_.back());
            games_.pop_back();
        }
    }
}

void Server::DeleteClients() {
    //todo delete clients
    UpdateFDMax();
    clients_to_delete_.resize(0);
}

void Server::TryToStartGame() {
    for(auto & inf : infos_) {
        if(inf.opponentIP.empty() && inf.client->CanReceive()) {
            Message m(inf.client->Receive());
            inf.opponentIP = m.opponentIP;
        }
    }
    for(int i = 0; i < infos_.size(); i++) {
        for(int j = i + 1; j < infos_.size(); j++) {
            if(infos_[i].inGame || infos_[j].inGame || infos_[i].opponentIP.empty() || infos_[j].opponentIP.empty())
                continue;
            if(infos_[i].opponentIP == infos_[j].ip && infos_[i].ip == infos_[j].opponentIP){
                spdlog::info("Starting new game between {} and {}", infos_[i].ip, infos_[j].ip);
                infos_[i].inGame = true;
                infos_[j].inGame = true;
                games_.emplace_back(
                    std::make_unique<Game>(infos_[i].client, infos_[j].client, new ServerBoard())
                );
            }
        }
    }
}