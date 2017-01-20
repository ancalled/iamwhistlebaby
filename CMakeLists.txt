CMAKE_MINIMUM_REQUIRED(VERSION 3.6)
PROJECT(whistle)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

SET(SOURCE_FILES VarianceTree.cpp PitchDetector.cpp Synthesizer.cpp SimpleMessageDecoder.cpp VarienceMessageDecoder.cpp WhistleConfig.h CRC32.h)

ADD_SUBDIRECTORY(whistle_tests)

#IF (LINUX)
    ADD_SUBDIRECTORY(linux)
#ENDIF()

IF (APPLE)
    ADD_SUBDIRECTORY(macos)
ENDIF()

SET(VERSION 1)
SET(BUILD_NUMBER $ENV{BUILD_NUMBER})
CONFIGURE_FILE("${PROJECT_SOURCE_DIR}/WhistleConfig.h.in" "${PROJECT_SOURCE_DIR}/WhistleConfig.h")


ADD_LIBRARY(whistle ${SOURCE_FILES})


#TARGET_LINK_LIBRARIES(whistle)
#TARGET_LINK_LIBRARIES(whistle ITKCommon)



ADD_EXECUTABLE(treetest treetest.cpp ${SOURCE_FILES})
#TARGET_LINK_LIBRARIES(treetest whistle)

ENABLE_TESTING()
ADD_TEST(NAME gtests COMMAND whistle_tests/run_whistle_tests)
