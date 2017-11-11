cmake_minimum_required(VERSION 3.7.1)

project(helloworld)

include_directories(${CMAKE_HOME_DIRECTORY}/include)

aux_source_directory(src SRCS)

add_executable(${PROJECT_NAME} ${SRCS})

if( DEFINED UNIX AND NOT DEFINED APPLE )
target_link_libraries(${PROJECT_NAME} uuid)
endif( DEFINED UNIX AND NOT DEFINED APPLE )

if( DEFINED WIN32  )
    set_target_properties( ${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/wd4566" )
endif( DEFINED WIN32 )

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/build/tests/${PROJECT_NAME})

