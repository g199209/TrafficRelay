//
// Created by Gao Mingfei on 2021/5/23.
//

#ifndef RELAY_COMMON_H_
#define RELAY_COMMON_H_

#include <unistd.h>

#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>

// Enable all loglevel in compile time
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <gflags/gflags.h>
#include <spdlog/spdlog.h>

#include "error_define.h"

#endif  // RELAY_COMMON_H_
