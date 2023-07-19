cmake_minimum_required(VERSION 3.22)
include(cmake/SetVersion.cmake)
project(regmap-manager LANGUAGES CXX VERSION ${VERSION})

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "source/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_subdirectory(tests)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
include(cmake/CreatePackage.cmake)
