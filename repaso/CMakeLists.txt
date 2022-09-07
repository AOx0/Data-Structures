cmake_minimum_required(VERSION 3.10)
project(repaso)

set(CMAKE_CXX_STANDARD 14)

add_executable(repaso 
  main.cpp 
  lifo.hpp
  lifo.cpp
  lib/option.hpp 
  lib/result.hpp
)
