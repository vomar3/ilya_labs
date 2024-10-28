cmake_minimum_required(VERSION 3.26)
project(ilya_laba1 C)

set(CMAKE_C_STANDARD 11)

add_executable(ilya_laba1 laba1/task1/task1.c)

add_executable(ilya3 laba1/task3/task3.c)

add_executable(ilya4 laba1/task4/task4.c)

add_executable(ilya10 laba1/task10/task10.c)

add_executable(ilya8 laba1/task8/task8.c)

add_executable(ilya9_1 laba1/task9/task9_1.c)

add_executable(ilya9_2 laba1/task9/task9_2.c)

add_executable(ilya7 laba1/task7/task7.c)

add_executable(ilya6 laba1/task6/task6.c)

add_executable(ilya5 laba1/task5/task5.c)

add_executable(ilya2 laba1/task2/task2.c)

add_executable(ilya1.2 laba2/task1/task1.c)

add_executable(ilya2.2 laba2/task2/task2.c)

add_executable(ilya2.7 laba2/task7/task7.c)

add_executable(ilya2.3 laba2/task3/task3.c)

add_executable(ilya2.9 laba2/task9/task9.c)

add_executable(ilya2.4 laba2/task4/task4.c)

add_executable(ilya2.8 laba2/task8/task8.c)

add_executable(ilya2.10 laba2/task10/task10.c)

add_executable(ilya3.1 laba3/task1/task1.c)

target_link_libraries(ilya_laba1 m)
