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
    std::function<void(bool, const std::vector<Ogre::Vector3>,
      Ogre::Vector3, bool, int, int)>
    ReceiveHandler;
  typedef std::function<void()> BeginTurnHandler;
  typedef std::function<void()> DidSendHitHandler;

  Client();

  void connect(std::string hostname,
    int port,
    std::function<void(bool)> completionCallback);
  void continuouslyReceiveBallPositions(
    ReceiveHandler receiveCallback,
    BeginTurnHandler beginTurnCallback);
  void continuouslyReceiveDebugHeartbeat();
  void sendHit(int strength, Ogre::Vector3 direction, int ballIdx,
    DidSendHitHandler callback);
  bool connected() const;
};

