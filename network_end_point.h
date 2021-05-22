//
// Created by Gao Mingfei on 2021/5/23.
//

#ifndef RELAY_NETWORK_END_POINT_H_
#define RELAY_NETWORK_END_POINT_H_

#include <arpa/inet.h>
#include <netinet/in.h>

#include <sstream>

#include "common.h"

namespace Relay {

struct NetworkEndPoint {
 public:
  enum class Dir {
    kDirListen = 0,
    kDirConnect = 1,
  };

  NetworkEndPoint(std::string_view ep_addr, std::string_view ep_proto,
                  std::string_view ep_dir) {
    auto pos = ep_addr.find(':');
    if (pos == std::string_view::npos) {
      SPDLOG_CRITICAL("Configure EP Address format error! {}", ep_addr.data());
      assert(false);
    }
    std::string ip_addr(ep_addr.substr(0, pos));
    uint16_t port = std::stoi(std::string(ep_addr.substr(pos + 1)));

    memset(&sock_addr_, 0, sizeof(sock_addr_));
    sock_addr_.sin_family = AF_INET;
    sock_addr_.sin_addr.s_addr = inet_addr(ip_addr.c_str());
    sock_addr_.sin_port = htons(port);

    if (ep_proto == "TCP" || ep_proto == "tcp") {
      protocol_ = IPPROTO_TCP;
    } else if (ep_proto == "UDP" || ep_proto == "udp") {
      protocol_ = IPPROTO_UDP;
    } else {
      SPDLOG_CRITICAL("Configure EP Proto WRONG! {}", ep_proto);
      assert(false);
    }

    if (ep_dir == "Listen" || ep_dir == "listen") {
      dir_ = Dir::kDirListen;
    } else if (ep_dir == "Connect" || ep_dir == "connect") {
      dir_ = Dir::kDirConnect;
    } else {
      SPDLOG_CRITICAL("Configure EP Dir WRONG! {}", ep_dir);
      assert(false);
    }

    std::ostringstream oss;
    oss << "EP[" << ep_proto;
    if (dir_ == Dir::kDirConnect) {
      oss << " Connect to " << ep_addr;
    } else if (dir_ == Dir::kDirListen) {
      oss << " Listen at " << ep_addr;
    } else {
      assert(false);
    }
    oss << "]";
    str_ = oss.str();
  }

  NetworkEndPoint(const NetworkEndPoint&) = default;

  struct sockaddr_in sock_addr_;
  int protocol_;
  Dir dir_;
  std::string str_;
};

}  // namespace Relay
#endif  // RELAY_NETWORK_END_POINT_H_
