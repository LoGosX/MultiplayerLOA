#include "networking/server.h"

#include "spdlog/spdlog.h"

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>


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

    std::vector<int> cfds_to_delete;
    for(auto & [cfd, client] : clients_) {
        spdlog::info("cfd={} can_receive={} can_send={}\n", cfd, client->CanReceive(), client->CanSend());
        if(client->CanReceive()) {
            spdlog::info("Waiting for message from {}\n", cfd);
            ByteBuffer buffer = client->Receive();
            spdlog::info("Buffer.size() = {}", buffer.GetSize());
            if(buffer.IsEmpty()){
                cfds_to_delete.push_back(cfd);
                continue;
            }
            spdlog::info("Got {} from cfd={}\n", buffer.ReadString(), cfd); 
        }
        if(client->CanSend()) {
            spdlog::info("Sending message to {}\n", cfd);
            client->Send(ByteBuffer("Hello from server\n"));
        }
    }
    for(auto cfd : cfds_to_delete)
        RemoveClient(cfd);
}

void Server::RemoveClient(int cfd) {
    clients_.erase(cfd);
    close(cfd);
    FD_CLR(cfd, &rmask_);
    FD_CLR(cfd, &wmask_);
    UpdateFDMax();
}