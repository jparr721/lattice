//
// Created by Jarred Parr on 3/23/21.
//

#include <lattice/socket.h>

#include <iostream>
#include <unistd.h>

Socket::Socket() {
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    assert(sockfd > -1);

    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    std::strncpy(server_addr.sun_path, socket_path.c_str(), sizeof(server_addr.sun_path) - 1);

    if ((connect(sockfd, reinterpret_cast<sockaddr*>(&server_addr),
                 sizeof(server_addr))) < 0) {
        close(sockfd);
        perror("socket connect");
        exit(1);
    }

    signal(SIGPIPE, SIG_IGN);
}

float Socket::Predict(float k, float x) {
    const std::string input = std::to_string(k) + "," + std::to_string(x);
    if ((write(sockfd, input.c_str(), input.size())) < 0) {
        perror("write");
        exit(1);
    }

    if ((read(sockfd, buffer, sizeof(buffer))) < 0) {
        perror("read");
        exit(1);
    }

    const std::string output = std::string(buffer);

    // Clear the buffer once we're done with it
    std::memset(&buffer, 0, sizeof(buffer));

    return std::stof(output);
}