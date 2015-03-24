#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <exception>
#include "mail.pb.h"

using boost::asio::ip::tcp;

int main(int argc, char* argv[]) {
  try {
    boost::asio::io_service io_service;
    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 57432));

    tcp::socket socket(io_service);

    std::cout << "Waiting for a client..." << std::endl;
    acceptor.accept(socket);

    std::cout << "Connected to client!" << std::endl;

    while (true) {
      boost::system::error_code err;

      int messageSize;
      boost::asio::read(socket, boost::asio::buffer(&messageSize, sizeof(int)), err);
      
      if (err == boost::asio::error::eof) {
        std::cout << "Client closed connection" << std::endl;
        break;
      }

      std::vector<uint8_t> protobufRaw(messageSize);
      boost::asio::read(socket, boost::asio::buffer(&protobufRaw[0], messageSize));
      
      mail m;
      if (m.ParseFromArray(&protobufRaw[0], messageSize)) {
        std::cout << "-----BEGIN MESSAGE-----" << std::endl;
        std::cout << "Sender: " << m.sender() << std::endl;
        std::cout << "Content: " << m.content() << std::endl;
        std::cout << "------END MESSAGE------" << std::endl;
      } else {
        throw std::runtime_error("Could not read client mail, closing connection");
      }
    }
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
