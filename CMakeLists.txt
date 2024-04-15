cmake_minimum_required(VERSION 3.27)
project(PikachuGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")

set(asset "${CMAKE_SOURCE_DIR}/asset")
set(build_asset "${CMAKE_CURRENT_BINARY_DIR}/asset")

add_executable(${PROJECT_NAME} src/main.cpp src/main.h src/utils.h src/console.h src/controller.h
    src/drawer.h
    src/coord.h
    src/constant.h
    src/bfs.h
    src/board.h
    src/audio.h
)

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD COMMAND ${CMAKE_COMMAND}
    -E create_symlink ${asset} ${build_asset}
    DEPENDS ${build_asset}
    COMMENT "symbolic link ${asset} => ${build_asset}"
)