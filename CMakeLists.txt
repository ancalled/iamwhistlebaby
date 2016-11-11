cmake_minimum_required(VERSION 3.6)
project(AudioProtocol)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Coder.cpp Coder.h Decoder.cpp Decoder.h)
add_executable(AudioProtocol ${SOURCE_FILES})