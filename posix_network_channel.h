//
// Created by Gao Mingfei on 2021/5/23.
//

#ifndef RELAY_POSIX_NETWORK_CHANNEL_H_
#define RELAY_POSIX_NETWORK_CHANNEL_H_

#include "network_channel.h"

namespace Relay {

class PosixNetworkChannel : public NetworkChannel {
 public:
  explicit PosixNetworkChannel(const NetworkEndPoint& ep)
      : NetworkChannel(ep) {}

  err_t Start() override;
  err_t Send(const void* buf, size_t len) override;
  err_t Recv(void* buf, size_t buf_size, size_t* len) override;
  err_t WaitReady() override;

 private:
  int sock_;  // This sock may refer to listen socket or connect socket
  int fd_;    // Use this fd to do IO
};

}  // namespace Relay
#endif  // RELAY_POSIX_NETWORK_CHANNEL_H_
