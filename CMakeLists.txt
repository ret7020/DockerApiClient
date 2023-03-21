cmake_minimum_required(VERSION 2.8)	 # Cmake min version

project(docker_api)

find_package(CURL REQUIRED)
find_package(fmt REQUIRED)

set(SOURCE_EXE main.cpp)
set(SOURCE_LIB docker.cpp)
set(HEADER docker.h)

add_library(docker STATIC ${SOURCE_LIB})	

add_executable(main ${SOURCE_EXE})

target_link_libraries(main docker)

target_link_libraries(main CURL::libcurl)
target_link_libraries(main fmt::fmt)