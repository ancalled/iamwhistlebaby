CMAKE_MINIMUM_REQUIRED(VERSION 3.6)
PROJECT(whistle_tests)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -std=gnu++11")

ADD_SUBDIRECTORY(lib/gtest-1.7.0)
INCLUDE_DIRECTORIES(../ ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

ADD_EXECUTABLE(run_whistle_tests WhistleTest.cc stringutil.h)
ADD_EXECUTABLE(decode_file decode_file.cpp stringutil.h)
ADD_EXECUTABLE(decode_file2 decode_file2.cpp stringutil.h)

TARGET_LINK_LIBRARIES(run_whistle_tests gtest gtest_main whistle)
TARGET_LINK_LIBRARIES(decode_file whistle)
TARGET_LINK_LIBRARIES(decode_file2 whistle)
