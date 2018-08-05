project(gamedev CXX)
cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

include("$ENV{CMAKI_PWD}/node_modules/npm-mas-mas/cmaki/cmaki.cmake")
cmaki_find_package(dune-glew)
cmaki_find_package(sdl2)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
set(Boost_DEBUG 0)
find_package(Boost COMPONENTS coroutine context stacktrace_addr2line log random REQUIRED)

add_definitions(-D_GNU_SOURCE)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -framework OpenGL")
# set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -framework OpenGL")

include_directories(${Boost_INCLUDE_DIRS})
include_directories(include/entt/src)

cmaki_executable(example src/example.cpp DEPENDS ${Boost_LIBRARIES} dl)
# target_link_libraries(example ${Boost_LIBRARIES} dl)
install(DIRECTORY ${CONAN_LIB_DIRS_BOOST}/ DESTINATION ${CMAKE_BUILD_TYPE})
install(TARGETS example DESTINATION ${CMAKE_BUILD_TYPE})

