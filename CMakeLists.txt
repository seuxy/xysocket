cmake_minimum_required(VERSION 3.8)
project(xysocket)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp
                 WebSocketServer.cpp WebSocketServer.h
                 Util.cpp Util.h
                 sonWebSocketServer.cpp sonWebSocketServer.h)
add_executable(xysocket ${SOURCE_FILES})
target_link_libraries(xysocket /usr/local/lib/libwebsockets.so)
