#include "net/TcpServer.h"

#include <iostream>
#include <memory>
#include <string>

namespace asio = boost::asio;
using asio::ip::tcp;

namespace {
struct Session : std::enable_shared_from_this<Session> {
  explicit Session(tcp::socket socket) : socket_(std::move(socket)) {}

  void start() { do_read(); }

 private:
  void do_read() {
    auto self = shared_from_this();

    asio::async_read_until(
        socket_, buffer_, '\n',
        [self](const boost::system::error_code& ec, std::size_t) {
          if (ec) {
            return;
          }

          std::istream is(&self->buffer_);
          std::string line;
          std::getline(is, line);

          if (!line.empty() && line.back() == '\r') {
            line.pop_back();
          }

          self->response_ = "OK\n";
          self->do_write();
        });
  }

  void do_write() {
    auto self = shared_from_this();

    asio::async_write(socket_, asio::buffer(response_),
                      [self](const boost::system::error_code& ec, std::size_t) {
                        if (ec) {
                          return;
                        }

                        self->do_read();
                      });
  }

 private:
  tcp::socket socket_;
  asio::streambuf buffer_;
  std::string response_;
};
}  // namespace

TcpServer::TcpServer(asio::io_context& io, unsigned short port)
    : io_(io), acceptor_(io) {
  tcp::endpoint ep(tcp::v4(), port);
  acceptor_.open(ep.protocol());
  acceptor_.set_option(tcp::acceptor::reuse_address(true));
  acceptor_.bind(ep);
  acceptor_.listen();
}

void TcpServer::start() { do_accept(); }

void TcpServer::stop() {
  boost::system::error_code ec;
  acceptor_.close(ec);
}

void TcpServer::do_accept() {
  acceptor_.async_accept(
      [this](const boost::system::error_code& ec, tcp::socket socket) {
        if (!ec) {
          std::make_shared<Session>(std::move(socket))->start();
        }

        if (acceptor_.is_open()) {
          do_accept();
        }
      });
}