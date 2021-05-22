//
// Created by Gao Mingfei on 2021/5/23.
//

#ifndef RELAY_ERROR_DEFINE_H_
#define RELAY_ERROR_DEFINE_H_

namespace Relay {

enum err_t : int32_t {
  RelayOK = 0,
  RelayNetworkError = -1,
  RelayNotInit = -2,
  RelayMultiCall = -3,
};

}  // namespace Relay

#endif  // RELAY_ERROR_DEFINE_H_
