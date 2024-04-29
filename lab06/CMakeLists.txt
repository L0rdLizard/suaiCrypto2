# cmake_minimum_required(VERSION 3.12)


# project(lab06)
# include(FetchContent)

# find_package(OpenSSL REQUIRED)

# add_executable(lab06 main.cpp)

# target_link_libraries(lab06 OpenSSL::SSL OpenSSL::Crypto)


cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

project(lab06)

file(DOWNLOAD
    https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.38.7/CPM.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

add_executable(lab06 main.cpp)

CPMAddPackage(
    NAME Boost
    VERSION 1.84.0
    URL https://github.com/boostorg/boost/releases/download/boost-1.84.0/boost-1.84.0.tar.xz
    URL_HASH SHA256=2e64e5d79a738d0fa6fb546c6e5c2bd28f88d268a2a080546f74e5ff98f29d0e
    OPTIONS "BOOST_ENABLE_CMAKE ON" "BOOST_INCLUDE_LIBRARIES multiprecision"
)

target_link_libraries(lab06 PRIVATE Boost::multiprecision)
