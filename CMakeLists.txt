CMAKE_MINIMUM_REQUIRED(VERSION 3.6)
PROJECT(whistle)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

SET(SOURCE_FILES PitchDetector.cpp PitchDetector.h Synthesizer.cpp Synthesizer.h Decoder.cpp Decoder.h)

ADD_SUBDIRECTORY(whistle_tests)
ADD_SUBDIRECTORY(media_tests)

ADD_LIBRARY(whistle ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(whistle)
#TARGET_LINK_LIBRARIES(whistle ITKCommon)