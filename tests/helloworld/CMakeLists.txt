cmake_minimum_required(VERSION 3.7.1)

project(helloworld)

include_directories(${CMAKE_HOME_DIRECTORY}/include)

aux_source_directory(src SRCS)

add_executable(${PROJECT_NAME} ${SRCS})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/build/tests/${PROJECT_NAME})


