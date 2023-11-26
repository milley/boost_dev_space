cmake_minimum_required(VERSION 3.27)
project(BOOST_DEV_SPACE VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_OSX_ARCHITECTURES "arm64")

#find_package(Boost REQUIRED COMPONENTS regex)

include_directories("/Users/milley/DevTools/boost/boost_1_83_0")

add_subdirectory(hello)