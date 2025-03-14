cmake_minimum_required(VERSION 3.28)
project(LLMChessTutor LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# Ensure assets/ exists in the source directory
if(EXISTS ${CMAKE_SOURCE_DIR}/assets)
    # Copy the entire assets/ directory to the build directory
    file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
endif()

# Automatically find all .cpp files in src/
file(GLOB SOURCES src/*.cpp)

add_executable(main ${SOURCES})

target_compile_features(main PRIVATE cxx_std_20)
target_link_libraries(main PRIVATE SFML::Graphics)