#pragma once

#include <boost/asio.hpp>
#include <thread>
#include <iostream>
#include "GameMessage.pb.h"

using boost::asio::ip::tcp;

class Server {
  boost::asio::io_service io_service;
  tcp::socket sock;
  bool connectStatus;

  GameMessage storage;

public:
  Server();

  void accept(int port, std::function<void()> completionCallback);
  bool connected() const;
};
