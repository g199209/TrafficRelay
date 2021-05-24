cmake_minimum_required(VERSION 3.0)
project(TrafficRelay)

set(CMAKE_CXX_STANDARD 17)

find_package(spdlog REQUIRED)
find_package(gflags REQUIRED)

include_directories("/usr/include")
include_directories("/usr/local/include")
include_directories("/opt/homebrew/include")

link_directories("/usr/lib")
link_directories("/usr/local/lib")
link_directories("/opt/homebrew/lib")

# Set a default build type for single-configuration
# CMake generators if no build type is set.
IF (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug)
ENDIF (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)

if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  set(CMAKE_CXX_FLAGS "-std=c++17 -Wall")
  set(LINK_LIBRARIES gflags fmt)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -march=x86-64 -static-libgcc -static-libstdc++")
  set(LINK_LIBRARIES libgflags.a libfmt.a pthread)
else ()
  message(FATAL_ERROR "Unsupported Compiler! ${CMAKE_CXX_COMPILER_ID}")
endif ()

set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb -O0 ")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -ggdb -O3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

message(STATUS "Build on ${CMAKE_CXX_COMPILER_ID}")

add_executable(TrafficRelay
    configure_flags.cc
    posix_network_channel.cc
    relay_thread.cc
    main.cc)

target_link_libraries(TrafficRelay ${LINK_LIBRARIES})