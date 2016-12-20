CMAKE_MINIMUM_REQUIRED(VERSION 3.6)
PROJECT(whistle_tests)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

SET(gtest_SOURCE_DIR /home/ancalled/Documents/distr/googletest/googletest)
INCLUDE_DIRECTORIES(../ ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
#INCLUDE_DIRECTORIES(../)

SET(SOURCE_FILES main.cpp)


ADD_EXECUTABLE(whistle_tests ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(whistle_tests whistle)
