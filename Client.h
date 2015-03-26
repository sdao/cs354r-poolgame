#pragma once

#include <boost/asio.hpp>
#include <thread>
#include <iostream>
#include "GameMessage.pb.h"

using boost::asio::ip::tcp;

class Client {
  boost::asio::io_service io_service;
  tcp::socket sock;
  bool connectStatus;

  GameMessage storage;

public:
  Client();

  void connect(std::string hostname,
    int port,
    std::function<void(bool)> completionCallback);
  bool connected() const;
};

