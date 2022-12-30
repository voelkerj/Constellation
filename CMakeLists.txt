cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules/")

#set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG ON)

project(Constellation)
add_executable(Constellation src/main.cpp src/satellite.cpp src/math_helpers.cpp src/input_parser.cpp)

# Get SDL Packages
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

# Get Threads Package
find_package(Threads REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES) #strip out trailing whitespace to avoid CMAKE error
target_link_libraries(Constellation ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} Threads::Threads)