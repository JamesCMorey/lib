cmake_minimum_required(VERSION 3.15)
project(lib)

add_library(ll STATIC src/ll.c)
target_include_directories(ll PUBLIC include)

#add_subdirectory(lib)
#add_executable(a.out main.c)
#target_link_libraries(a.out PRIVATE ll)

# mkdir build && cd build && cmake .. && cmake --build .
