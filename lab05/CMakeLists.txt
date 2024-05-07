cmake_minimum_required(VERSION 3.12)

# set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

project(lab05)
include(FetchContent)

find_package(OpenSSL REQUIRED)

add_executable(lab05 lab05.cpp)


#FetchContent_Declare(
#        openssl-cmake
#        URL https://github.com/jimmy-park/openssl-cmake/archive/main.tar.gz
#)
#FetchContent_MakeAvailable(openssl-cmake)
#
#add_executable(lab05 RSA.cpp)
#target_link_libraries(lab05 PRIVATE
#        OpenSSL::SSL
#        OpenSSL::Crypto
#        OpenSSL::applink
#)


target_link_libraries(lab05 OpenSSL::SSL OpenSSL::Crypto)