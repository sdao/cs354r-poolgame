#include "Client.h"


Client::Client()
  : io_service(),
    sock(io_service),
    connectStatus(false) {}

void Client::connect(std::string hostname,
  int port,
  std::function<void()> completionCallback)
{
  std::thread([&]() {
    std::cout << "Looking for server...\n";
    tcp::resolver resolver(io_service);
    tcp::resolver::query query(hostname, std::to_string(port));
    tcp::resolver::iterator iter = resolver.resolve(query);
    boost::asio::connect(sock, iter);
    std::cout << "Server connected!\n";
    connectStatus = true;
    completionCallback();
  });
}

bool Client::connected() const {
  return connectStatus;
}

