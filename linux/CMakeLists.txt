cmake_minimum_required(VERSION 3.6)
project(whistle-media-test)

set(CMAKE_CXX_STANDARD 11)

INCLUDE_DIRECTORIES(/usr/local/include ../)
LINK_DIRECTORIES(/usr/local/lib)

ADD_EXECUTABLE(linux_synth linux_synth.cpp)
ADD_EXECUTABLE(linux_listen linux_listen.cpp)
ADD_EXECUTABLE(recorder record.cpp)
ADD_EXECUTABLE(player play.cpp)


TARGET_LINK_LIBRARIES(linux_synth whistle pulse pulse-simple)
TARGET_LINK_LIBRARIES(linux_listen whistle pulse pulse-simple)
TARGET_LINK_LIBRARIES(recorder whistle pulse pulse-simple)
TARGET_LINK_LIBRARIES(player whistle pulse pulse-simple)

