cmake_minimum_required(VERSION 3.3.2)

project(xgraphical)
set(SRCS "")

include_directories(${CMAKE_HOME_DIRECTORY}/include)
include_directories(${CMAKE_HOME_DIRECTORY}/xgraphical)

if( BE_BASED_ON_OPENGL )
    if(OS_LINUX)
        aux_source_directory(opengl/linux SRCS)
        add_library( ${PROJECT_NAME} ${SRCS} )
        target_link_libraries(${PROJECT_NAME} GL)
    elseif(OS_WINDOWS)
        aux_source_directory(opengl/windows SRCS)
        add_library( ${PROJECT_NAME} ${SRCS} )
        target_link_libraries(${PROJECT_NAME} opengl32)
    endif()
endif()


