#include "Server.h"

Server::Server() : io_service(),
                   sock(io_service),
                   connectStatus(false) {}

void Server::accept(int port, std::function<void()> completionCallback) {
  std::thread t([&]() {
    std::cout << "Waiting for client...\n";

    {
      std::lock_guard<std::mutex> lock(mutex);
      tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));
      acceptor.accept(sock);
    }

    std::cout << "Client connected!\n";
    connectStatus = true;
    completionCallback();
  });
  t.detach();
}

void
Server::postBallPositions(const std::vector<Ogre::Vector3>& ballPositions) {
  storage.set_type(GameMessage_Type_BALL_POSITIONS);
  storage.clear_ball_positions();
  storage.clear_score();
  storage.clear_client_hit();

  auto ballMessage = storage.mutable_ball_positions();
  for (const Ogre::Vector3& v : ballPositions) {
    Vector3f* vMsg = ballMessage->add_ball();
    vMsg->set_x(v.x);
    vMsg->set_y(v.y);
    vMsg->set_z(v.z);
  }

  std::thread t([&]() {
    std::lock_guard<std::mutex> lock(mutex);
    int size = ballMessage->ByteSize();
    std::vector<std::uint8_t> data(sizeof(int) + size);
    *reinterpret_cast<int*>(&data[0]) = size;
    ballMessage->SerializeToArray(&data[sizeof(int)], size);
    boost::asio::write(sock, boost::asio::buffer(data));
  }); 
}

bool Server::connected() const {
  return connectStatus;
}

