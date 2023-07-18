cmake_minimum_required(VERSION 3.25)
project(PR6 C)

set(CMAKE_C_STANDARD 11)

# add_executable(PR6 Client.c)
add_executable(PR6 Server.c)

if(MINGW)
    target_link_libraries(PR6 ws2_32)
endif()