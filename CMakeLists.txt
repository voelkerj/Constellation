cmake_minimum_required(VERSION 3.10)

set(SDL2_DIR "C:/Libraries/SDL2-2.26.2")
set(SDL2_image_DIR "C:/Libraries/SDL2_image-2.6.2")

set(SDL2_INCLUDE_DIRS "${SDL2_DIR}/x86_64-w64-mingw32/include/SDL2")
set(SDL2_LIBRARIES "${SDL2_DIR}/x86_64-w64-mingw32/lib/libSDL2main.a;${SDL2_DIR}/x86_64-w64-mingw32/lib/libSDL2.dll.a")
set(SDL2_IMAGE_INCLUDE_DIRS "${SDL2_image_DIR}/x86_64-w64-mingw32/include/SDL2")
set(SDL2_IMAGE_LIBRARIES "${SDL2_image_DIR}/x86_64-w64-mingw32/lib/libSDL2_image.dll.a")

project(Constellation)

add_executable(Constellation 
    src/main.cpp
    src/input_parser.cpp
    src/math_helpers.cpp
    src/satellite.cpp
)

target_link_libraries(Constellation -lmingw32 ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

target_include_directories(Constellation 
    PUBLIC ${SDL2_INCLUDE_DIRS}
    PUBLIC ${SDL2_IMAGE_INCLUDE_DIRS}
)