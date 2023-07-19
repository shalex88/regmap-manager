cmake_minimum_required(VERSION 3.22)
include(cmake/SetVersion.cmake)
project(regmap-manager LANGUAGES CXX VERSION ${VERSION})

set(CMAKE_CXX_STANDARD 17)

# Include the CMake script for generating the register map
include(resources/registers_map/GenerateRegistersMap.cmake)

include_directories(include)
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "source/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_subdirectory(tests)

# Make the executable depend on the custom target
add_dependencies(${PROJECT_NAME} generate_registers_map)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
include(cmake/CreatePackage.cmake)
