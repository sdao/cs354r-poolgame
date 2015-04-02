#include "Server.h"

#include <chrono>

Server::Server() : io_service(),
                   sock(io_service),
                   connectStatus(false) {}

void Server::accept(int port, std::function<void()> completionCallback) {
  std::thread t([=]() {
    std::cout << "Waiting for client...\n";

    {
      std::lock_guard<std::mutex> lock(this->mutex);
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
    const Ogre::Vector3& cueBallPosition,
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
  auto cueBallPos = ballMessage->mutable_cue_ball();
  cueBallPos->set_x(cueBallPosition.x);
  cueBallPos->set_y(cueBallPosition.y);
  cueBallPos->set_z(cueBallPosition.z);
  ballMessage->set_make_noise(makeNoise);
  ballMessage->set_host_score(hostScore);
  ballMessage->set_client_score(clientScore);

  std::thread t([=]() {
    std::lock_guard<std::mutex> lock(this->mutex);
    int size = this->storage.ByteSize();
//std::cout << "size: " << size << "\n";
    std::vector<std::uint8_t> data(sizeof(int) + size);
    *reinterpret_cast<int*>(&data[0]) = size;
    this->storage.SerializeToArray(&data[sizeof(int)], size);
    boost::asio::write(this->sock, boost::asio::buffer(data));
  });
  t.detach(); 
}

void Server::endHostTurn() {
  storage.set_type(GameMessage_Type_SERVER_RELEASE_CONTROL);
  storage.clear_ball_positions();
  storage.clear_client_hit();

  std::thread t([this]() {
    std::lock_guard<std::mutex> lock(this->mutex);
    int size = this->storage.ByteSize();
    std::vector<std::uint8_t> data(sizeof(int) + size);
    *reinterpret_cast<int*>(&data[0]) = size;
    this->storage.SerializeToArray(&data[sizeof(int)], size);
    boost::asio::write(this->sock, boost::asio::buffer(data));
  });
  t.detach();
}

void Server::waitForClientHit(
  std::function<void(int, Ogre::Vector3)> completionCallback
) {
  std::thread t([=]() {
    boost::system::error_code error;

    int messageSize;
    boost::asio::read(this->sock,
      boost::asio::buffer(&messageSize, sizeof(int)),
      error);

    if (error == boost::asio::error::eof) {
      std::cout << "Connection forcibly closed by client\n";
      completionCallback(-1, Ogre::Vector3::ZERO);
      return;
    }

    std::vector<uint8_t> protobufRaw(messageSize);
    boost::asio::read(this->sock,
      boost::asio::buffer(&protobufRaw[0], messageSize));
    
    if (storage.ParseFromArray(&protobufRaw[0], messageSize)) {
      if (storage.type() == GameMessage_Type_CLIENT_HIT) {
        std::cout << "Server's turn again!\n";
        auto hitMessage = storage.client_hit();
        int strength = hitMessage.strength();
        auto dir = hitMessage.direction();
        completionCallback(strength,
          Ogre::Vector3(dir.x(), dir.y(), dir.z()));
        return;
      } else {
        completionCallback(-1, Ogre::Vector3::ZERO);
        return;
      }
    }
  });
  t.detach();
}

void Server::debugHeartbeat() {
  std::thread t([this]() {
    int counter = 0;
    while (true) {
      std::vector<Ogre::Vector3> empty;
      postBallPositions(empty, Ogre::Vector3::ZERO, false, counter, counter);
      std::cout << "----Heartbeat---\n";
      std::this_thread::sleep_for(std::chrono::seconds(1));
      counter++;
    }
  });
  t.detach();
}

bool Server::connected() const {
  return connectStatus;
}

