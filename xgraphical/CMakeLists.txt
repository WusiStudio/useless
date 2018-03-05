cmake_minimum_required(VERSION 3.3.2)

project(xgraphical)
set(SRCS "")

include_directories(${CMAKE_HOME_DIRECTORY}/include)
include_directories(${CMAKE_HOME_DIRECTORY}/xgraphical)

if(BE_BASED_ON_OPENGL)
    aux_source_directory(opengl SRCS)
endif()

add_library( ${PROJECT_NAME} ${SRCS} )

if(BE_BASED_ON_OPENGL AND (CMAKE_SYSTEM_NAME MATCHES "Windows"))
    target_link_libraries(${PROJECT_NAME} opengl32)
elseif( BE_BASED_ON_OPENGL AND (CMAKE_SYSTEM_NAME MATCHES "Linux"))
    target_link_libraries(${PROJECT_NAME} GL)
endif()
