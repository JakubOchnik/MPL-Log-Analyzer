cmake_minimum_required(VERSION 3.0.0)
project(mpl_log_parser VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
#set(TEST_DIR ${CMAKE_SOURCE_DIR}/test)

find_package(Boost COMPONENTS program_options date_time REQUIRED)
include_directories(${INCLUDE_DIR})


add_executable(mpl_log_parser main.cpp)
add_subdirectory(src)
target_link_libraries(
    mpl_log_parser 
    PUBLIC utils Boost::program_options Boost::date_time 
    INTERFACE castUtils
    PRIVATE Boost::headers)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

#include(CTest)
#enable_testing()
#add_subdirectiry(${TEST_DIR})