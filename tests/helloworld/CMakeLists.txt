cmake_minimum_required(VERSION 3.3.2)

project(helloworld)

include_directories(${CMAKE_HOME_DIRECTORY}/deps)
include_directories(${CMAKE_HOME_DIRECTORY}/include)
include_directories(${CMAKE_HOME_DIRECTORY}/tests/helloworld/include)

set(SRCS "")
aux_source_directory(src SRCS)

add_executable(${PROJECT_NAME} ${SRCS})

# if ( CMAKE_SYSTEM_NAME MATCHES "Linux" )
# target_link_libraries(${PROJECT_NAME} uuid X11 Xxf86vm)
# endif( CMAKE_SYSTEM_NAME MATCHES "Linux" )

target_link_libraries( ${PROJECT_NAME} useless )

if( CMAKE_SYSTEM_NAME MATCHES "Windows" )
    #屏蔽字符集错误
    set_target_properties( ${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/wd4566" )
    #不显示控制台窗口(会屏蔽终端输出信息)
    #set_target_properties( ${PROJECT_NAME} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup" )
    set_target_properties( ${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup" )
endif( CMAKE_SYSTEM_NAME MATCHES "Windows" )

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/build/tests/${PROJECT_NAME})

