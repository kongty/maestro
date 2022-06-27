cmake_minimum_required(VERSION 3.10)
project(maestro)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# turn every warnings on
set(CMAKE_CXX_FLAGS "-Wall -Wextra -fPIC")

add_subdirectory(extern/pybind11)
add_subdirectory(cost-model)
add_subdirectory(python)
