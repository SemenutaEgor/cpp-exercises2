#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <chrono>
#include <cstring>
#include <stdexcept>
#include <string>

inline int connect_with_timeout(const char* ip, uint16_t port, int timeout_ms) {
 auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeout_ms);

 while (std::chrono::steady_clock::now() < deadline) {
  int fd = ::socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) throw std::runtime_error("socket() failed");

  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  if (::inet_pton(AF_INET, ip, &addr.sin_addr) != 1) {
   ::close(fd);
   throw std::runtime_error("inet_pton failed");
  }

  if (::connect(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == 0) {
   return fd;
  }

  ::close(fd);
 }

 throw std::runtime_error("connect timeout");
}

inline void send_all(int fd, const std::string& data) {
 const char* p = data.data();
 size_t left = data.size();
 while(left > 0) {
  ssize_t n = ::send(fd, p, left, 0);
  if (n < 0) throw std::runtime_error(std::string("send failed: ") + std::strerror(errno));
  p += n;
  left -= static_cast<size_t>(n);
 }
}

inline std::string recv_line(int fd) {
 std::string out;
 out.reserve(256);

 char c;
 while (true) {
  ssize_t n = ::recv(fd, &c, 1, 0);
  if (n == 0) throw std::runtime_error("server closed connection");
  if (n < 0) throw std::runtime_error(std::string("recv failed: ") + std::strerror(errno));
  if (c == '\n') break;
  if (c != '\r') out.push_back(c);
 }

 return out;
}

inline std::string send_request(const char* ip, uint16_t port,
const std::string& line, int timeout_ms = 2000) {
 int fd = connect_with_timeout(ip, port, timeout_ms);
 try {
  send_all(fd, line + "\n");
  auto resp = recv_line(fd);
  ::close(fd);
  return resp;
 } catch (...) {
  ::close(fd);
  throw;
 }
}