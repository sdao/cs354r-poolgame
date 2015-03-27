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
      tcp::resolver resolver(this->io_service);
      tcp::resolver::query query(hostname, std::to_string(port));
      tcp::resolver::iterator iter = resolver.resolve(query);

      boost::system::error_code error;
      boost::asio::connect(this->sock, iter, error);

      if (error) {
        std::cout << "Could not connect :(\n";
        std::cout << error.message() << "\n";
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

void Client::continuouslyReceiveBallPositions(
  ReceiveHandler receiveCallback,
  BeginTurnHandler beginTurnCallback
) {
  std::thread t([=]() {
    while (true) {
      boost::system::error_code error;

      int messageSize;
      boost::asio::read(this->sock,
        boost::asio::buffer(&messageSize, sizeof(int)),
        error);

      if (error == boost::asio::error::eof) {
        std::cout << "Connection forcibly closed by server\n";
        receiveCallback(false, std::vector<Ogre::Vector3>(), false, 0, 0);
        return;
      }

      std::vector<uint8_t> protobufRaw(messageSize);
      boost::asio::read(this->sock,
        boost::asio::buffer(&protobufRaw[0], messageSize));
    
      if (storage.ParseFromArray(&protobufRaw[0], messageSize)) {
        if (storage.type() == GameMessage_Type_SERVER_RELEASE_CONTROL) {
          std::cout << "Server released control; client's time to shine!\n";
          beginTurnCallback();
          return;
        } else if (storage.type() == GameMessage_Type_BALL_POSITIONS) {
          std::cout << "Server sent update info\n";
          auto ballMessage = storage.ball_positions();
          std::vector<Ogre::Vector3> data;
          for (int i = 0; i < ballMessage.ball_size(); ++i) {
            auto v = ballMessage.ball(i);
            data.push_back(Ogre::Vector3(v.x(), v.y(), v.z()));
          }
          receiveCallback(true,
            data,
            ballMessage.make_noise(),
            ballMessage.host_score(),
            ballMessage.client_score());
          return;
        }
      }
    }
  });
  t.detach();
}

void Client::continuouslyReceiveDebugHeartbeat() {
  continuouslyReceiveBallPositions([](
    bool success,
    const std::vector<Ogre::Vector3> pos,
    bool makeNoise,
    int hostScore,
    int clientScore
  ) {
    if (success) {
      std::cout << "(" << hostScore << ") Heartbeat received\n";
    } else {
      std::cout << "Error while receiving heartbeat\n";
    }
  }, []() {});
}

void Client::sendHit(int strength, Ogre::Vector3 direction, int ballIdx) {
  storage.set_type(GameMessage_Type_CLIENT_HIT);
  storage.clear_ball_positions();
  storage.clear_client_hit();
  
  auto hitMessage = storage.mutable_client_hit();
  if (strength == 1) {
    hitMessage->set_strength(HitInfo_Strength_LOW);
  } else if (strength == 2) {
    hitMessage->set_strength(HitInfo_Strength_MEDIUM);
  } else if (strength == 3) {
    hitMessage->set_strength(HitInfo_Strength_HIGH);
  } else {
    assert(false && "hit strength was not 1, 2, or 3");
  }
  auto hitPos = hitMessage->mutable_direction();
  hitPos->set_x(direction.x);
  hitPos->set_y(direction.y);
  hitPos->set_z(direction.z);
  hitMessage->set_ball_index(ballIdx);

  std::thread t([this]() {
    int size = this->storage.ByteSize();
    std::vector<std::uint8_t> data(sizeof(int) + size);
    *reinterpret_cast<int*>(&data[0]) = size;
    this->storage.SerializeToArray(&data[sizeof(int)], size);
    boost::asio::write(this->sock, boost::asio::buffer(data));
  });
  t.detach();
}

bool Client::connected() const {
  return connectStatus;
}

