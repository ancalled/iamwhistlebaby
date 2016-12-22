CMAKE_MINIMUM_REQUIRED(VERSION 3.6)
PROJECT(whistle)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

SET(SOURCE_FILES PitchDetector.cpp PitchDetector.h Synthesizer.cpp Synthesizer.h Decoder.cpp Decoder.h WhistleConfig.h)

ADD_SUBDIRECTORY(whistle_tests)
ADD_SUBDIRECTORY(media_tests)

SET(VERSION 1)
SET(BUILD_NUMBER $ENV{build.number})
CONFIGURE_FILE("${PROJECT_SOURCE_DIR}/WhistleConfig.h.in" "${PROJECT_SOURCE_DIR}/WhistleConfig.h")


ADD_LIBRARY(whistle ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(whistle)
#TARGET_LINK_LIBRARIES(whistle ITKCommon)