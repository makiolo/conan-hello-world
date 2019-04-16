project(gamedev CXX)
cmake_minimum_required(VERSION 3.7.2)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

include("$ENV{CMAKI_PWD}/node_modules/npm-mas-mas/cmaki/cmaki.cmake")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
if(CMAKE_BUILD_TYPE MATCHES Debug)
	set(Boost_DEBUG 1)
else()
	set(Boost_DEBUG 0)
endif()
find_package(Boost COMPONENTS coroutine context stacktrace_addr2line log random REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(include/entt/src)
install(DIRECTORY ${CONAN_LIB_DIRS_BOOST}/ DESTINATION ${CMAKE_BUILD_TYPE})

add_definitions(-D_GNU_SOURCE)
cmaki_executable(conan-hello-world src/game.cpp DEPENDS ${CONAN_LIBS} ${Boost_LIBRARIES} dl)

