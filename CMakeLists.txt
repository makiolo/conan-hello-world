project(gamedev CXX)
cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

function(cmaki_install_files FROM)
	file(COPY ${FROM} DESTINATION .)
endfunction()

set(Boost_DEBUG 1)
find_package(Boost COMPONENTS coroutine context stacktrace_addr2line log random REQUIRED)

add_definitions(-D_GNU_SOURCE)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(entt/src)
add_executable(example example.cpp)
target_link_libraries(example ${Boost_LIBRARIES})
target_link_libraries(example dl)
cmaki_install_files("${CONAN_LIB_DIRS_BOOST}")

