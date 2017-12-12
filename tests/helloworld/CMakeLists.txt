cmake_minimum_required(VERSION 3.7.1)

project(helloworld)

include_directories(${CMAKE_HOME_DIRECTORY}/deps)
include_directories(${CMAKE_HOME_DIRECTORY}/include)

set(SRCS "")
aux_source_directory(src SRCS)

add_executable(${PROJECT_NAME} ${SRCS})

# if ( CMAKE_SYSTEM_NAME MATCHES "Linux" )
# target_link_libraries(${PROJECT_NAME} uuid X11 Xxf86vm)
# endif( CMAKE_SYSTEM_NAME MATCHES "Linux" )

if( CMAKE_SYSTEM_NAME MATCHES "Windows" )
    set_target_properties( ${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/wd4566" )
endif( CMAKE_SYSTEM_NAME MATCHES "Windows" )

target_link_libraries( ${PROJECT_NAME} useless )

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/build/tests/${PROJECT_NAME})

