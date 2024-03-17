cmake_minimum_required(VERSION 3.12)

project(lab05)

find_package(OpenSSL REQUIRED)

add_executable(lab05 main.cpp RSAfun.cpp)

target_link_libraries(lab05 OpenSSL::SSL OpenSSL::Crypto)