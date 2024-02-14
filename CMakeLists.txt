cmake_minimum_required(VERSION 3.5.1)  # <3.5 no longer accepted
project(ASSIGNMENT VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()

set(CMAKE_C_STANDARD 11)

# Specify the directory containing source files
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)

# Add the source files to the project
add_executable(ASSIGNMENT 
    ${SOURCE_DIR}/main.c 
    ${SOURCE_DIR}/myString.h 
    ${SOURCE_DIR}/transac.h
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
