cmake_minimum_required(VERSION 3.16.0)

project(emu6502)

set(CMAKE_CXX_STANDARD 17)

set(ROOT_DIR "${PROJECT_SOURCE_DIR}/../src")
set(6502_DIR "${ROOT_DIR}/6502")
set(SYSTEM_DIR "${ROOT_DIR}/nessystem")

include_directories(
    ${6502_DIR}
    ${SYSTEM_DIR}
)

set(SOURCES
    ${6502_DIR}/cpu6502.cpp
    ${6502_DIR}/bus.cpp

    ${SYSTEM_DIR}/nessystem.cpp

    ${ROOT_DIR}/main.cpp
)

set(INCLUDES
    ${6502_DIR}/bus.h
    ${6502_DIR}/cpu6502.h
    ${SYSTEM_DIR}/nessystem.h
)

add_executable(emu6502
    ${SOURCES}
    ${INCLUDES}
)