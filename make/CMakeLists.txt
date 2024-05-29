cmake_minimum_required(VERSION 3.16.0)

project(emu6502)

set(CMAKE_CXX_STANDARD 17)

set(ROOT_DIR "${PROJECT_SOURCE_DIR}/../src")
set(6502_DIR "${ROOT_DIR}/6502")
set(SYSTEM_DIR "${ROOT_DIR}/nessystem")
set(DEBUG_DIR "${ROOT_DIR}/debug")
set(CPU_DEBUG_DIR "${DEBUG_DIR}/cpu")

find_package(Curses REQUIRED)
include_directories(
    ${6502_DIR}
    ${SYSTEM_DIR}
    ${CURSES_INCLUDE_DIR}
)

set(NES_SOURCES
    ${6502_DIR}/cpu6502.cpp

    ${SYSTEM_DIR}/bus.cpp
    ${SYSTEM_DIR}/nessystem.cpp

    ${ROOT_DIR}/main.cpp
)

set(NES_INCLUDES
    ${SYSTEM_DIR}/bus.h
    ${6502_DIR}/cpu6502.h
    ${SYSTEM_DIR}/nessystem.h
)

set(CPU_DEBUG_SOURCES
    ${6502_DIR}/cpu6502.cpp
    ${SYSTEM_DIR}/bus.cpp
    ${CPU_DEBUG_DIR}/CPUDebug.cpp
)

set(CPU_DEBUG_INCLUDES
    ${SYSTEM_DIR}/bus.h
    ${6502_DIR}/cpu6502.h
)

add_executable(CPUDebug
    ${CPU_DEBUG_SOURCES}
    ${CPU_DEBUG_INCLUDES}
)

target_link_libraries(CPUDebug ${CURSES_LIBRARY})

add_executable(emuNES
    ${NES_SOURCES}
    ${NES_INCLUDES}
)


