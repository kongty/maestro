# Boost package
find_package(Boost REQUIRED COMPONENTS program_options filesystem system)

# maestro library
file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.hpp")
add_library(maestro-core ${HEADERS} src/BASE_base-objects.cpp)

target_include_directories(maestro-core PUBLIC
                           ${CMAKE_CURRENT_SOURCE_DIR}/include
                           ${CMAKE_CURRENT_SOURCE_DIR}/include/base
                           ${CMAKE_CURRENT_SOURCE_DIR}/include/tools
                           ${CMAKE_CURRENT_SOURCE_DIR}/include/user-api
                           ${CMAKE_CURRENT_SOURCE_DIR}/include/dataflow-analysis
                           ${CMAKE_CURRENT_SOURCE_DIR}/include/dataflow-specification-language
                           ${CMAKE_CURRENT_SOURCE_DIR}/include/design-space-exploration
                           ${CMAKE_CURRENT_SOURCE_DIR}/include/cost-analysis
                           ${CMAKE_CURRENT_SOURCE_DIR}/include/abstract-hardware-model
                           )

target_link_libraries(maestro-core Boost::program_options Boost::filesystem Boost::system)

# turn all the warnings on
target_compile_options(maestro-core PRIVATE -Wall -Wextra)
