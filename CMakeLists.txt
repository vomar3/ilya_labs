cmake_minimum_required(VERSION 3.26)
project(ilya_laba1 C)

set(CMAKE_C_STANDARD 11)

add_executable(ilya_laba1 laba1/task1/task1.c)

add_executable(ilya3 laba1/task3/task3.c)

add_executable(ilya4 laba1/task4/task4.c)

add_executable(ilya10 laba1/task10/task10.c)

add_executable(ilya8 laba1/task8/task8.c)

target_link_libraries(ilya_laba1 m)
