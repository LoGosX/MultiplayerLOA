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
        clients_[cfd] = std::make_unique<ServerClient>(cfd);
        if (cfd > fdmax_)
            fdmax_ = cfd;
    }
    else
    {
        for(auto & [cfd, client] : clients_)
        {
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
    for(auto & [cfd, client] : clients_) {
        spdlog::info("cfd={} can_receive={} can_send={}\n", cfd, client->CanReceive(), client->CanSend());
        if(client->CanReceive()) {
            ParseMessage(client.get());
        }
    }

    DeleteClients();

    sleep(1);
}

void Server::ParseMessage(ServerClient * client) {
    ByteBuffer buffer = client->Receive();
    if(buffer.IsEmpty()) {
        spdlog::info("{} disconnected", client->GetFd());
        MarkClientToDelete(client->GetFd());
    }else {
        Message message(buffer);
        spdlog::info("Got {} \n", message.ToString());
        if(message.type == Type::kSearchingForGame) {
            clients_looking_for_game_.push_back({
                {client, message.name, message.opponentName}
            });
        }else {
            spdlog::warn("{} parse not handled yet", static_cast<int>(message.type));
        }
    }
}

void Server::MarkClientToDelete(int c) {
    clients_to_delete_.push_back(c);
} 

void Server::DeleteClients() {
    for(int cfd : clients_to_delete_) {
        clients_.erase(cfd);
        close(cfd);
        FD_CLR(cfd, &rmask_);
        FD_CLR(cfd, &wmask_);
    }
    UpdateFDMax();
    clients_to_delete_.resize(0);
}

void Server::TryToStartGame() {
    std::vector<int> ids_game_started;
    for(int i = 0; i < clients_looking_for_game_.size(); i++) {
        for(int j = i + 1; j < clients_looking_for_game_.size(); j++) {
            auto info1 = clients_looking_for_game_[i];
            auto info2 = clients_looking_for_game_[j];
            if(info1.who == info2.targetName && info2.who == info1.targetName) {
                ids_game_started.push_back(i);
                ids_game_started.push_back(j);
                ServerBoard * board = new ServerBoard()
                Game * game = new Game() 
            }
        }
    }
}