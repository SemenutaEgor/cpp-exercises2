#pragma once

#include <functional>
#include <boost/asio.hpp>

class TcpServer {
 public:
  using RequestHandler = std::function<std::string(const std::string&)>;

  TcpServer(boost::asio::io_context&, unsigned short, RequestHandler);

  void start();
  void stop();

 private:
  void do_accept();

  boost::asio::io_context& io_;
  boost::asio::ip::tcp::acceptor acceptor_;
  RequestHandler handler_;
};