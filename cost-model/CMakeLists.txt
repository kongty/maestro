# Boost package
find_package(Boost REQUIRED COMPONENTS program_options filesystem system)
add_library(maestro ${HEADERS} src/BASE_base-objects.cpp)

target_include_directories(maestro PRIVATE
                           ./include
                           ./include/base
                           ./include/tools
                           ./include/user-api
                           ./include/dataflow-analysis
                           ./include/dataflow-specification-language
                           ./include/design-space-exploration
                           ./include/cost-analysis
                           ./include/abstract-hardware-model
                           )

target_link_libraries(maestro Boost::program_options Boost::filesystem Boost::system)

# turn all the warnings on
if (MSVC)
  target_compile_options(maestro PRIVATE /W4 /WX)
else()
  target_compile_options(maestro PRIVATE -Wall -Wextra -Werror)
endif()
