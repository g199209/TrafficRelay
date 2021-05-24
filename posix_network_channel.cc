//
// Created by Gao Mingfei on 2021/5/23.
//

#include "posix_network_channel.h"

#include <sys/socket.h>

namespace Relay {

err_t PosixNetworkChannel::Start() {
  if (init_) {
    SPDLOG_ERROR("{} Already Started!", ep_.str_);
    return RelayMultiCall;
  }

  // Only allow start once, so always set init_
  std::lock_guard lck(init_mtx_);
  init_ = true;
  if (ep_.protocol_ == IPPROTO_TCP) {
    sock_ = ::socket(AF_INET, SOCK_STREAM, ep_.protocol_);
  } else if (ep_.protocol_ == IPPROTO_UDP) {
    sock_ = ::socket(AF_INET, SOCK_DGRAM, ep_.protocol_);
  } else {
    assert(false);
  }

  if (ep_.dir_ == NetworkEndPoint::Dir::kDirListen) {
    int opt_val = 1;
    auto ret = ::setsockopt(sock_, SOL_SOCKET, SO_REUSEPORT, &opt_val,
                            sizeof(opt_val));
    if (ret != 0) {
      SPDLOG_ERROR("{} Reuse port error! errno: {}", ep_.str_, errno);
      init_ret_ = RelayNetworkError;
      return init_ret_;
    }
    ret = ::bind(sock_, (struct sockaddr *)&ep_.sock_addr_,
                 sizeof(ep_.sock_addr_));
    if (ret != 0) {
      SPDLOG_ERROR("{} Bind error! errno: {}", ep_.str_, errno);
      init_ret_ = RelayNetworkError;
      return init_ret_;
    }

    SPDLOG_INFO("{} Listen.....", ep_.str_);
    ::listen(sock_, 1);

    struct sockaddr_in peer_addr;
    socklen_t peer_addr_len = sizeof(peer_addr);
    fd_ = accept(sock_, (struct sockaddr *)&peer_addr, &peer_addr_len);
    if (fd_ < 0) {
      SPDLOG_ERROR("{} Accept error! errno: {}", ep_.str_, errno);
      init_ret_ = RelayNetworkError;
      return init_ret_;
    }
  } else if (ep_.dir_ == NetworkEndPoint::Dir::kDirConnect) {
    SPDLOG_INFO("{} Connect.....", ep_.str_);
    auto ret = ::connect(sock_, (struct sockaddr *)&ep_.sock_addr_,
                         sizeof(ep_.sock_addr_));
    if (ret != 0) {
      SPDLOG_ERROR("{} Connect error! errno: {}", ep_.str_, errno);
      init_ret_ = RelayNetworkError;
      return init_ret_;
    }

    fd_ = sock_;
  } else {
    assert(false);
  }

  SPDLOG_INFO("{} Start!", ep_.str_);
  init_ret_ = RelayOK;
  return init_ret_;
}

err_t PosixNetworkChannel::Send(const void *buf, size_t len) {
  if (!init_) {
    SPDLOG_ERROR("{} Not Start yet!", ep_.str_);
    return RelayNotInit;
  }

  while (len > 0) {
    auto n = ::write(fd_, buf, len);
    if (n < 0) {
      SPDLOG_ERROR("{} Send error! errno: {}", ep_.str_, errno);
      return RelayNetworkError;
    }
    SPDLOG_DEBUG("{} Send {} Bytes.", ep_.str_, n);
    len -= n;
    buf = (uint8_t *)buf + n;
  }

  return RelayOK;
}

err_t PosixNetworkChannel::Recv(void *buf, size_t buf_size, size_t *len) {
  if (!init_) {
    SPDLOG_ERROR("{} Not Start yet!", ep_.str_);
    return RelayNotInit;
  }

  auto n = ::read(fd_, buf, buf_size);
  if (n < 0) {
    SPDLOG_ERROR("{} Recv error! errno: {}", ep_.str_, errno);
    *len = 0;
    return RelayNetworkError;
  }
  SPDLOG_DEBUG("{} Recv {} Bytes.", ep_.str_, n);
  *len = n;

  return RelayOK;
}

err_t PosixNetworkChannel::WaitReady() {
  std::unique_lock lck(init_mtx_);
  init_cv_.wait(lck, [this]() { return init_; });
  return init_ret_;
}

}  // namespace Relay
