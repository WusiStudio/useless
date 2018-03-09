cmake_minimum_required(VERSION 3.3.2)

project(useless)

set(USELESS_VERSION_MAJOR "1")
set(USELESS_VERSION_MINOR "0")
set(USELESS_VERSION_PATCH "0")
set(USELESS_VERSION_EXTRA "")
set(USELESS_VERSION ${USELESS_VERSION_MAJOR}.${USELESS_VERSION_MINOR})
set(USELESS_VERSION_FULL ${USELESS_VERSION}.${USELESS_VERSION_PATCH}${USELESS_VERSION_EXTRA})

set(CMAKE_BUILD_TYPE Debug CACHE STRING "set build type to debug") 

option(BE_BASED_ON_OPENGL "" ON)
option(BE_BASED_ON_OPENGLES "" OFF)
option(BE_BASED_ON_VULKAN "" OFF)
option(BE_BASED_ON_DIRECT3D_12 "" OFF)

option(BUILD_UNIT_TESTS "" ON)

#设置编译选项-------------------------------------------
if ((CMAKE_SYSTEM_NAME MATCHES "Linux") OR (${CYGWIN}))
    set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -m64 -std=c++11 -O0 -Wall -g")
    set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -m64 -std=c++11 -O3 -Wall")
    add_compile_options(-DCHAR_SET="UTF-8")
    add_compile_options(-DOS_LINUX)
    option(OS_LINUX "" ON )
    aux_source_directory(src/linux SRCS)
elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")  
    set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS}")
    set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS}")
    add_compile_options(/source-charset:utf-8)
    add_compile_options(-DCHAR_SET="GBK")
    add_compile_options(-DOS_WINDOWS)
    option(OS_WINDOWS "" ON)
    aux_source_directory(src/windows SRCS)
elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -m64 -O0 -Wall -g")
    set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -m64 -O3 -Wall")
    add_compile_options(-std=c++11)
    add_compile_options(-Wall)
    add_compile_options(-m64)
    add_compile_options(-DCHAR_SET="UTF-8")
    add_compile_options(-DOS_Darwin)
    option(OS_Darwin "" ON)
    aux_source_directory(src/osx SRCS)
elseif (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
endif()

add_compile_options(-DROOT_NAMESPACE=ws)

include_directories(${CMAKE_HOME_DIRECTORY}/deps)
include_directories(${CMAKE_HOME_DIRECTORY}/include)
include_directories(${CMAKE_HOME_DIRECTORY}/xgraphical)
aux_source_directory(src SRCS)

message(${SRCS})

add_subdirectory(xgraphical)

add_library(${PROJECT_NAME} ${SRCS})

if(OS_LINUX)
    target_link_libraries(${PROJECT_NAME} uuid xcb X11 X11-xcb)
endif(OS_LINUX)

target_link_libraries(${PROJECT_NAME} xgraphical)

if(BUILD_UNIT_TESTS)
    add_subdirectory(tests)
endif()



