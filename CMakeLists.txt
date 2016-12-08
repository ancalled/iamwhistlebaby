cmake_minimum_required(VERSION 3.6)
project(AudioProtocol)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp tablesynth.h tablesynth.cpp Decoder.cpp Decoder.h YinDecoder.cpp YinDecoder.h Segment.cpp Segment.h)
add_executable(AudioProtocol ${SOURCE_FILES})