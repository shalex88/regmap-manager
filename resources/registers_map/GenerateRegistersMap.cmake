set(GENERATED_OUTPUT_DIR ${CMAKE_BINARY_DIR}/registers_map)

# Create the directory for registers_map.h
file(MAKE_DIRECTORY ${GENERATED_OUTPUT_DIR})

set(GENERATOR ${CMAKE_CURRENT_LIST_DIR}/generate_registers_map.sh)
set(CSV ${CMAKE_CURRENT_LIST_DIR}/Registers.csv)
set(ENUMS_HEADER ${GENERATED_OUTPUT_DIR}/Registers.h)
set(MAP_HEADER ${GENERATED_OUTPUT_DIR}/RegistersMap.h)

# Specify the custom command to generate Registers.h and RegistersMap.h
add_custom_command(
	OUTPUT ${ENUMS_HEADER} ${MAP_HEADER}
    COMMAND bash ${GENERATOR} ${CSV} ${ENUMS_HEADER} ${MAP_HEADER}
    DEPENDS ${GENERATOR} ${CSV}
)

# Add a custom target and make the executable depend on this target
add_custom_target(
  generate_registers_map ALL
  DEPENDS ${ENUMS_HEADER} ${MAP_HEADER}
)

# Add the current source directory and the build directory's registers_map subdirectory to the list of paths to be searched for include files
include_directories(${GENERATED_OUTPUT_DIR})
