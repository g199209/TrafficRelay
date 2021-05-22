//
// Created by Gao Mingfei on 2021/5/23.
//

#include <gflags/gflags.h>

DEFINE_string(LogLevel, "info", "See SPDLOG_LEVEL_NAMES in spdlog/common.h");

DEFINE_string(FirstEpAddr, "127.0.0.1:5000", "First end point address");

DEFINE_string(FirstEpProto, "TCP", "First end point protocol, TCP or UDP");

DEFINE_string(FirstEpDir, "Listen",
              "First end point direction, Listen or Connect");

DEFINE_string(SecondEpAddr, "127.0.0.1:5001", "Second end point address");

DEFINE_string(SecondEpProto, "TCP", "Second end point protocol, TCP or UDP");

DEFINE_string(SecondEpDir, "Listen",
              "Second end point direction, Listen or Connect");
