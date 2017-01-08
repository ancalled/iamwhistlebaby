cmake_minimum_required(VERSION 3.6)
project(whistle-macos)

set(CMAKE_CXX_STANDARD 11)

INCLUDE_DIRECTORIES(/usr/local/include ../)
LINK_DIRECTORIES(/usr/local/lib)

ADD_EXECUTABLE(macaudio_synth macaudio_synth.cpp)
ADD_EXECUTABLE(macaudio_listen macaudio_listen.cpp)

# http://stackoverflow.com/questions/17070101/why-i-cannot-link-the-mac-framework-file-with-cmake
TARGET_LINK_LIBRARIES(macaudio_synth "-framework AudioToolbox" "-framework CoreFoundation" whistle)
SET_TARGET_PROPERTIES(macaudio_synth PROPERTIES LINK_FLAGS "-Wl,-F/Library/Frameworks")

TARGET_LINK_LIBRARIES(macaudio_listen "-framework AudioToolbox" "-framework CoreFoundation" whistle)
SET_TARGET_PROPERTIES(macaudio_listen PROPERTIES LINK_FLAGS "-Wl,-F/Library/Frameworks")