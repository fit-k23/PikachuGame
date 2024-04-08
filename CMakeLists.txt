cmake_minimum_required(VERSION 3.27)
project(PikachuGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")

link_directories(lib)

add_executable(PikachuGame main.cpp main.h
        utils.h
        search.h
        console.h)#miniaudio.h)

