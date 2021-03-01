cmake_minimum_required(VERSION 3.14)
project(dbell-mhapner C)

set(CMAKE_C_STANDARD 11)

add_executable(client client.c helpers.h helpers.c state.c state.h)
add_executable(server server.c helpers.h helpers.c state.c state.h)