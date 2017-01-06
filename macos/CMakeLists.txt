cmake_minimum_required(VERSION 3.6)
project(whistle-macos)

set(CMAKE_CXX_STANDARD 11)

INCLUDE_DIRECTORIES(/usr/local/include ../)
LINK_DIRECTORIES(/usr/local/lib)

ADD_EXECUTABLE(audiotest audiotest.cpp)

# http://stackoverflow.com/questions/17070101/why-i-cannot-link-the-mac-framework-file-with-cmake
TARGET_LINK_LIBRARIES(audiotest "-framework AudioToolbox" "-framework CoreFoundation" whistle)
set_target_properties(audiotest PROPERTIES LINK_FLAGS "-Wl,-F/Library/Frameworks")