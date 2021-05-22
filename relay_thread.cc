//
// Created by Gao Mingfei on 2021/5/23.
//

#include "relay_thread.h"

namespace Relay {

RelayThread::RelayThread(NetworkChannelPtr src_channel,
                         NetworkChannelPtr dst_channel)
    : src_channel_(std::move(src_channel)),
      dst_channel_(std::move(dst_channel)) {
  buf_ = ::malloc(kBufSize);
  assert(buf_);
  thread_ = std::thread([this]() { ThreadFun(); });
}

void RelayThread::ThreadFun() {
  size_t len;
  err_t ret;
  ret = src_channel_->Start();
  if (ret != RelayOK) {
    SPDLOG_ERROR("Channel Start error! Exit Relay thread!");
    return;
  }
  ret = dst_channel_->WaitReady();
  if (ret != RelayOK) {
    SPDLOG_ERROR("Another Channel Start error! Exit Relay thread!");
    return;
  }

  while (!exit_) {
    ret = src_channel_->Recv(buf_, kBufSize, &len);
    if (ret != RelayOK) {
      SPDLOG_ERROR("Recv error! Exit Relay thread!");
      return;
    }

    ret = dst_channel_->Send(buf_, len);
    if (ret != RelayOK) {
      SPDLOG_ERROR("Send error! Exit Relay thread!");
      return;
    }
  }
}

err_t RelayThread::Stop() {
  exit_ = true;
  return RelayOK;
}

RelayThread::~RelayThread() {
  auto ret = Stop();
  assert(ret == RelayOK);
  thread_.join();
  ::free(buf_);
}

}  // namespace Relay
