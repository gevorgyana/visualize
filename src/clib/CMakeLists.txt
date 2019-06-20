cmake_minimum_required(VERSION 3.14)
project(clib)

set(CMAKE_CXX_STANDARD 14)

add_library(clib SHARED library.cpp library.h b_tree.h)