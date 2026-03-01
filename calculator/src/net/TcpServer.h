#pragma once

#include <boost/asio.hpp>

class TcpServer {
 public:
  TcpServer(boost::asio::io_context& io, unsigned short port);

  void start();
  void stop();

 private:
  void do_accept();

  boost::asio::io_context& io_;
  boost::asio::ip::tcp::acceptor acceptor_;
};