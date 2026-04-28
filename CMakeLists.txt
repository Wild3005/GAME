cmake_minimum_required(VERSION 3.25)
project(MyGame)

# Ambil raylib dari luar folder
add_subdirectory(/home/ichbinwil/INSTALL/raylib raylib)

add_executable(main src/main.cpp)

target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(main raylib)