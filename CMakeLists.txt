CMAKE_MINIMUM_REQUIRED(VERSION 3.6)
PROJECT(whistle)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#ADD_DEFINITIONS(-DGNUPLOT_DEBUG)

SET(SOURCE_FILES main.cpp YinDecoder.cpp YinDecoder.h Segment.cpp Segment.h Synthesizer.cpp Synthesizer.h)

#ADD_EXECUTABLE(whistle ${SOURCE_FILES})
ADD_LIBRARY(whistle ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(whistle ITKCommon)