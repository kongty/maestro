cmake_minimum_required(VERSION 3.10)
project(maestro)

set(CMAKE_CXX_STANDARD 17)

# turn every warnings on
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -fPIC")

############# compiler optimization flags #############
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG(-funroll-all-loops UNROLL)
if (${UNROLL})
    # osx's clang doesn't support it (travis)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -funroll-all-loops")
endif()

CHECK_CXX_COMPILER_FLAG(-static-libgcc COMPILER_STATIC)
if (${COMPILER_STATIC})
    # we also need to hide all symbols, otherwise we will run into cxx string ABI issue
    # https://stackoverflow.com/a/57077456
    set(STATIC_FLAG "-static-libgcc -static-libstdc++")
    set(STATIC_FLAG ${STATIC_FLAG} -Wl,--exclude-libs,ALL)
else()
    set(STATIC_FLAG "")
endif()

############# external libraries #############
# add_subdirectory(extern/pybind11)

add_subdirectory(cost-model)
# add_subdirectory(python)
