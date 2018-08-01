
cmake_minimum_required(VERSION 2.8)
 
PROJECT(acwe)
 
add_executable(main MACOSX_BUNDLE acwe.c main.c)
 
target_link_libraries(acwe)
