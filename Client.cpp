#include "Client.h"

Client::Client()
  : io_service(),
    sock(io_service),
    connectStatus(false) {}

void Client::connect(std::string hostname,
  int port,
  std::function<void(bool)> completionCallback)
{
  std::cout << "doing something\n";
  std::thread t([=]() {
    std::cout << "Looking for server `" << hostname << "`...\n";

    try {
      tcp::resolver resolver(io_service);
      tcp::resolver::query query(hostname, std::to_string(port));
      tcp::resolver::iterator iter = resolver.resolve(query);

      boost::system::error_code error;
      boost::asio::connect(sock, iter, error);

      if (error) {
        std::cout << "Could not connect :(\n";
        connectStatus = false;
        completionCallback(false);
      } else {
        std::cout << "Server connected!\n";
        connectStatus = true;
        completionCallback(true);
      }
    } catch (...) {
      std::cout << "Could not resolve server :(\n";
      connectStatus = false;
      completionCallback(false);
    }
  });
  t.detach();
}

bool Client::connected() const {
  return connectStatus;
}

