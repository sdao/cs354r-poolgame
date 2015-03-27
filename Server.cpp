#include "Server.h"

#include <chrono>

Server::Server() : io_service(),
                   sock(io_service),
                   connectStatus(false) {}

void Server::accept(int port, std::function<void()> completionCallback) {
  std::thread t([=]() {
    std::cout << "Waiting for client...\n";

    {
      std::lock_guard<std::mutex> lock(mutex);
      std::cout << "Accepting client on port " << port << "\n";
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
Server::postBallPositions(
    const std::vector<Ogre::Vector3>& ballPositions,
    bool makeNoise,
    int hostScore,
    int clientScore
) {
  storage.set_type(GameMessage_Type_BALL_POSITIONS);
  storage.clear_ball_positions();
  storage.clear_client_hit();

  auto ballMessage = storage.mutable_ball_positions();
  for (const Ogre::Vector3& v : ballPositions) {
    Vector3f* vMsg = ballMessage->add_ball();
    vMsg->set_x(v.x);
    vMsg->set_y(v.y);
    vMsg->set_z(v.z);
  }

  ballMessage->set_make_noise(makeNoise);
  ballMessage->set_host_score(hostScore);
  ballMessage->set_client_score(clientScore);

  std::thread t([&]() {
    std::lock_guard<std::mutex> lock(mutex);
    int size = ballMessage->ByteSize();
    std::vector<std::uint8_t> data(sizeof(int) + size);
    *reinterpret_cast<int*>(&data[0]) = size;
    ballMessage->SerializeToArray(&data[sizeof(int)], size);
    boost::asio::write(sock, boost::asio::buffer(data));
  }); 
}

void Server::debugHeartbeat() {
  std::thread t([this]() {
    while (true) {
      std::vector<Ogre::Vector3> empty;
      postBallPositions(empty, false, 42, 42);
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  });
  t.detach();
}

bool Server::connected() const {
  return connectStatus;
}

