cmake_minimum_required(VERSION 3.16)
project(ads_programm)

set(CMAKE_CXX_STANDARD 17)

add_executable(ads_programm main.cpp utils.h rankAndSelect.cpp rankAndSelect.h rangeMinimumQuerie.h eliasFano.h eliasFano.cpp rangeMinimumQuerie.cpp)
