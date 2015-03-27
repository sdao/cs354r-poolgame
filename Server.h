#pragma once

#include <boost/asio.hpp>
#include <thread>
#include <iostream>
#include <mutex>
#include <vector>
#include <atomic>
#include <OgreVector3.h>
#include "GameMessage.pb.h"

using boost::asio::ip::tcp;

class Server {
  boost::asio::io_service io_service;
  tcp::socket sock;
  std::mutex mutex;
  bool connectStatus;
  std::atomic<bool> attemptingAccept;

  GameMessage storage;

public:
  Server();

  void accept(int port, std::function<void()> completionCallback);
  void postBallPositions(
    const std::vector<Ogre::Vector3>& ballPositions,
    bool makeNoise,
    int hostScore,
    int clientScore
  );
  void endHostTurn();
  void waitForClientHit(
    std::function<void(int, Ogre::Vector3)> completionCallback);
  void debugHeartbeat();
  bool connected() const;
};

