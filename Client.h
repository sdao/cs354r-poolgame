#pragma once

#include <boost/asio.hpp>
#include <thread>
#include <iostream>
#include <OgreVector3.h>
#include "GameMessage.pb.h"

using boost::asio::ip::tcp;

class Client {
  boost::asio::io_service io_service;
  tcp::socket sock;
  bool connectStatus;

  GameMessage storage;

public:
  typedef
    std::function<void(bool, const std::vector<Ogre::Vector3>, bool, int, int)>
    ReceiveHandler;

  Client();

  void connect(std::string hostname,
    int port,
    std::function<void(bool)> completionCallback);
  void continuouslyReceiveBallPositions(ReceiveHandler receiveCallback);
  void continuouslyReceiveDebugHeartbeat();
  bool connected() const;
};

