cmake_minimum_required(VERSION 3.7.1)

project(xgraphical)
set(SRCS "")

include_directories(${CMAKE_HOME_DIRECTORY}/include)
include_directories(${CMAKE_HOME_DIRECTORY}/xgraphical)

if(BE_BASED_ON_OPENGL)
    aux_source_directory(opengl SRCS)
endif()

add_library( ${PROJECT_NAME} ${SRCS} )

if(BE_BASED_ON_OPENGL)
target_link_libraries(${PROJECT_NAME} opengl32)
endif()
