cmake_minimum_required(VERSION 3.16.0)

project(emu6502)

set(CMAKE_CXX_STANDARD 17)

set(6502_ROOT_DIR "${PROJECT_SOURCE_DIR}/../src")
set(6502_SRC_DIR "${6502_ROOT_DIR}/6502/src")
set(6502_INCLUDE_DIR "${6502_ROOT_DIR}/6502/include")

set(6502_SOURCES
    ${6502_SRC_DIR}/bus.cpp
    ${6502_SRC_DIR}/cpu.cpp

    ${6502_ROOT_DIR}/main.cpp
)

set(6502_INCLUDES
    ${6502_INCLUDE_DIR}/bus.h
    ${6502_INCLUDE_DIR}/cpu.h
)

add_executable(emu6502
    ${6502_SOURCES}
    ${6502_INCLUDES}
)