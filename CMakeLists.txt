project(gamedev CXX)
cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(Boost_DEBUG 0)
find_package(Boost COMPONENTS coroutine context stacktrace_addr2line log random REQUIRED)

# include($ENV{CMAKI_PWD}/node_modules/npm-mas-mas/cmaki/cmaki.cmake)
# cmaki_find_package(json)

add_definitions(-D_GNU_SOURCE)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(include/entt/src)
add_executable(example src/example.cpp)
target_link_libraries(example ${Boost_LIBRARIES} dl)
install(DIRECTORY ${CONAN_LIB_DIRS_BOOST}/ DESTINATION .)
install(TARGETS example DESTINATION .)

