cmake_minimum_required(VERSION 3.16.0)

project(emu6502test)

set(CMAKE_CXX_STANDARD 17)

set(ROOT_DIR "${PROJECT_SOURCE_DIR}/../src")

find_package(Curses REQUIRED)
include_directories(
    ${ROOT_DIR}
)

set(CPU_SOURCES
    ${ROOT_DIR}/mos6502.cpp
    ${ROOT_DIR}/bus.cpp
    ${ROOT_DIR}/CPUDebug.cpp
)

set(CPU_INCLUDES
    ${ROOT_DIR}/bus.h
    ${ROOT_DIR}/mos6502.h
)

add_executable(cpucomp
    ${CPU_SOURCES}
    ${CPU_INCLUDES}
)

target_link_libraries(cpucomp ${CURSES_LIBRARY})



