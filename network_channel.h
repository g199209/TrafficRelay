//
// Created by Gao Mingfei on 2021/5/23.
//

#ifndef RELAY_NETWORK_CHANNEL_H_
#define RELAY_NETWORK_CHANNEL_H_

#include "common.h"
#include "network_end_point.h"

namespace Relay {

class NetworkChannel {
 public:
  explicit NetworkChannel(const NetworkEndPoint& ep) : ep_(ep){};
  ~NetworkChannel() = default;

  NetworkChannel(const NetworkChannel&) = delete;
  NetworkChannel& operator=(const NetworkChannel&) = delete;

  virtual err_t Start() = 0;

  virtual err_t Send(const void* buf, size_t len) = 0;

  virtual err_t Recv(void* buf, size_t buf_size, size_t* len) = 0;

  virtual err_t WaitReady() = 0;

 protected:
  NetworkEndPoint ep_;

  bool init_ = false;
  err_t init_ret_ = RelayNotInit;
  std::mutex init_mtx_;
  std::condition_variable init_cv_;
};

using NetworkChannelPtr = std::shared_ptr<NetworkChannel>;

}  // namespace Relay
#endif  // RELAY_NETWORK_CHANNEL_H_
