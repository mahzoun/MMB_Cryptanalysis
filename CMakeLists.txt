cmake_minimum_required(VERSION 3.15)
project(MMB_Cryptanalysis)

set(CMAKE_CXX_STANDARD 14)

add_executable(MMB_Cryptanalysis main.cpp MMB.cpp MMB.h)