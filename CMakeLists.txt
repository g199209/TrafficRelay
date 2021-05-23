cmake_minimum_required(VERSION 3.0)
project(TrafficRelay)

set(CMAKE_CXX_STANDARD 17)

find_package(spdlog REQUIRED)
find_package(gflags REQUIRED)

include_directories("/usr/include")
include_directories("/usr/local/include")

link_directories("/usr/lib")
link_directories("/usr/local/lib")

# Set a default build type for single-configuration
# CMake generators if no build type is set.
IF (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug)
ENDIF (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -march=native -static-libgcc -static-libstdc++")
set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb -O0 ")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -ggdb -O3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  set(CMAKE_CXX_FLAGS "-std=c++17 -Wall")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -march= -static-libgcc -static-libstdc++")
else ()
  message(FATAL_ERROR "Unsupported Compiler! ${CMAKE_CXX_COMPILER_ID}")
endif ()

message(STATUS "Build on ${CMAKE_CXX_COMPILER_ID}")

add_executable(TrafficRelay
    configure_flags.cc
    posix_network_channel.cc
    relay_thread.cc
    main.cc)

target_link_libraries(TrafficRelay libgflags.a libspdlog.a libfmt.a)