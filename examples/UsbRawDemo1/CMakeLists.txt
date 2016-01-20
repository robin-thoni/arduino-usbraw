cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(PROJECT_NAME usbraw)
project(${PROJECT_NAME})

set(${CMAKE_PROJECT_NAME}_BOARD nano328)
set(${CMAKE_PROJECT_NAME}_PORT /dev/ttyUSB0)

enable_language(ASM)

set(${CMAKE_PROJECT_NAME}_SKETCH main.ino)
generate_arduino_firmware(${CMAKE_PROJECT_NAME})
