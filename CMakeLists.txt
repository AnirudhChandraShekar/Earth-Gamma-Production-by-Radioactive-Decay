cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(EarthGamma)

# Find Geant4
find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source files
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# Executable
add_executable(EarthGamma EarthGamma.cc ${sources} ${headers})
target_link_libraries(EarthGamma ${Geant4_LIBRARIES})

# Copy mac files to build directory
set(EARTHGAMMA_SCRIPTS
    init_vis.mac
    run.mac
    batch.mac
)
foreach(_script ${EARTHGAMMA_SCRIPTS})
    configure_file(
        ${PROJECT_SOURCE_DIR}/${_script}
        ${PROJECT_BINARY_DIR}/${_script}
        COPYONLY
    )
endforeach()
