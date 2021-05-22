//
// Created by Gao Mingfei on 2021/5/23.
//

#ifndef RELAY_RELAY_THREAD_H_
#define RELAY_RELAY_THREAD_H_

#include <thread>

#include "common.h"
#include "network_channel.h"

namespace Relay {

class RelayThread {
 public:
  RelayThread(NetworkChannelPtr src_channel, NetworkChannelPtr dst_channel);
  ~RelayThread();

  RelayThread(const RelayThread&) = delete;
  RelayThread& operator=(const RelayThread&) = delete;

  err_t Stop();

  inline void Join() { thread_.join(); }

 private:
  void ThreadFun();

 private:
  NetworkChannelPtr src_channel_;
  NetworkChannelPtr dst_channel_;

  std::thread thread_;
  bool exit_ = false;

  static constexpr size_t kBufSize = 16 * 1024 * 1024;
  void* buf_;
};

}  // namespace Relay
#endif  // RELAY_RELAY_THREAD_H_
