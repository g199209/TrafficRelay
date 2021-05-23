//
// Created by Gao Mingfei on 2021/5/23.
//

#include <csignal>
#include <list>

#include "common.h"
#include "network_channel.h"
#include "posix_network_channel.h"
#include "relay_thread.h"

DECLARE_string(LogLevel);
DECLARE_string(FirstEpAddr);
DECLARE_string(FirstEpProto);
DECLARE_string(FirstEpDir);
DECLARE_string(SecondEpAddr);
DECLARE_string(SecondEpProto);
DECLARE_string(SecondEpDir);

using namespace Relay;

namespace {

std::vector<NetworkChannelPtr> channels;
std::list<RelayThread> relay_threads;

void InitLog() {
  // Add TID
  spdlog::set_pattern("%t %+");
  spdlog::set_level(spdlog::level::from_str(FLAGS_LogLevel));
}

void CreateChannel() {
  SPDLOG_INFO("===== Create Channel =====");
  channels.push_back(std::make_shared<PosixNetworkChannel>(NetworkEndPoint(
      FLAGS_FirstEpAddr, FLAGS_FirstEpProto, FLAGS_FirstEpDir)));
  channels.push_back(std::make_shared<PosixNetworkChannel>(NetworkEndPoint(
      FLAGS_SecondEpAddr, FLAGS_SecondEpProto, FLAGS_SecondEpDir)));
}

void CreateThread() {
  SPDLOG_INFO("===== Create Thead =====");
  relay_threads.emplace_back(channels[0], channels[1]);
  relay_threads.emplace_back(channels[1], channels[0]);
}

void WaitThread() {
  for (auto& t : relay_threads) {
    t.Join();
  }
}

[[maybe_unused]] void SigHandle(int signo) {
  if (signo == SIGINT) {
    SPDLOG_WARN("Receive Signal SIGINT, exit!");
  } else if (signo == SIGQUIT) {
    SPDLOG_WARN("Receive Signal SIGQUIT, exit!");
  } else {
    SPDLOG_WARN("Receive Unhandled Signal {}!", signo);
    return;
  }

  for (auto& t : relay_threads) {
    t.Stop();
  }
}
}  // namespace

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, false);

  InitLog();
  SPDLOG_INFO("===== Program Start =====");

  CreateChannel();

  CreateThread();

  //  auto ret = signal(SIGINT, SigHandle);
  //  if (ret == SIG_ERR) {
  //    SPDLOG_ERROR("Set Signal SIGINT Handle Failed! errno: {}", errno);
  //    return errno;
  //  }
  //  ret = signal(SIGQUIT, SigHandle);
  //  if (ret == SIG_ERR) {
  //    SPDLOG_ERROR("Set Signal SIGQUIT Handle Failed! errno: {}", errno);
  //    return errno;
  //  }

  SPDLOG_INFO("===== Program Running...... =====");

  WaitThread();

  SPDLOG_INFO("===== Program Exit =====");
  return 0;
}
