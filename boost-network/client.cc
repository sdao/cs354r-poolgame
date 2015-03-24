#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <exception>
#include "mail.pb.h"

using boost::asio::ip::tcp;

int main(int argc, char* argv[]) {
  try {
    if (argc < 2) {
      throw std::runtime_error("No server specified");
    }

    boost::asio::io_service io_service;
    tcp::resolver resolver(io_service);
    tcp::resolver::query query(argv[1], "57432");
    tcp::resolver::iterator iter = resolver.resolve(query);

    std::cout << "Connecting to server..." << std::endl;
    tcp::socket socket(io_service);
    boost::asio::connect(socket, iter);

    std::cout << "Connected to server!" << std::endl;

    std::cout << "Sender name: ";
    std::string sender;
    std::getline(std::cin, sender);

    while (true) {
      std::cout << "Message content: ";
      std::string content;
      std::getline(std::cin, content);

      if (content.empty()) {
        break;
      }

      mail m;
      m.set_sender(sender);
      m.set_content(content);

      int messageSize = m.ByteSize();
      boost::asio::write(socket, boost::asio::buffer(&messageSize, sizeof(int)));

      std::vector<uint8_t> protobufRaw(messageSize);
      m.SerializeToArray(&protobufRaw[0], messageSize);
      boost::asio::write(socket, boost::asio::buffer(protobufRaw));
    }

    std::cout << "Closing connection" << std::endl;
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
