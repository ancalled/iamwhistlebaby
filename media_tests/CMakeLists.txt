cmake_minimum_required(VERSION 3.6)
project(whistle-media-test)

set(CMAKE_CXX_STANDARD 11)

INCLUDE_DIRECTORIES(/usr/local/include ../)
LINK_DIRECTORIES(/usr/local/lib)

ADD_EXECUTABLE(media_encodetest encode_test.cpp)
ADD_EXECUTABLE(media_decodetest decode_test.cpp)

TARGET_LINK_LIBRARIES(media_encodetest whistle pulse pulse-simple)
TARGET_LINK_LIBRARIES(media_decodetest whistle pulse pulse-simple)
