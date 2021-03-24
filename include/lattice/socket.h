//
// Created by Jarred Parr on 3/23/21.
//

#pragma once

#include <array>
#include <sys/socket.h>
#include <sys/un.h>
#include <string>

class Socket {
  public:
    const std::string socket_path = "../../py/hookenet";

    Socket();

    float Predict(float k, float x);

  private:
    int sockfd;

    sockaddr_un server_addr{};

    char buffer[128];
};
