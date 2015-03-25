#include "Server.h"

Server::Server() : io_service(),
                           sock(io_service),
                           connectStatus(false) {}

void Server::accept(int port, std::function<void()> completionCallback) {
  std::thread([&]() {
    std::cout << "Waiting for client...\n";
    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));
    acceptor.accept(sock);
    std::cout << "Client connected!\n";
    connectStatus = true;
    completionCallback();
  });
}

bool Server::connected() const {
  return connectStatus;
}

