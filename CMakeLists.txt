cmake_minimum_required(VERSION 3.7.1)

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

include_directories(uuid)

#设置编译选项-------------------------------------------
if(WIN32)
    set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS}")
    set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS}")
elseif(APPLE)
    set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -m64 -O0 -Wall -g")
    set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -m64 -O3 -Wall")
    add_compile_options(-std=c++11)
    add_compile_options(-Wall)
    add_compile_options(-m64)
elseif(UNIX)
    set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -m64 -std=c++11 -O0 -Wall -g")
    set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -m64 -std=c++11 -O3 -Wall")
endif()

if(BUILD_UNIT_TESTS)
    add_subdirectory(tests)
endif()

add_subdirectory(src)





