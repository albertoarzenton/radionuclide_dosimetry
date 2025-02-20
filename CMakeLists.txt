################################################################################
# Setup the project
################################################################################

cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(RADIONUCLIDE_DOSIMETRY)

################################################################################
# Set C++17 standard
################################################################################

# Enforce the use of C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

################################################################################
# Copy all scripts to the build directory i.e. the directory in which we build
# our application.
################################################################################

set(RADIONUCLIDE_DOSIMETRY_SCRIPTS
    macro/vis.mac
    macro/run.mac
  )

foreach(_script ${RADIONUCLIDE_DOSIMETRY_SCRIPTS})
  configure_file(
  ${PROJECT_SOURCE_DIR}/${_script}
  ${PROJECT_BINARY_DIR}/${_script}
  COPYONLY
  )
endforeach()

file(GLOB ROOT_MAP_FILES
  "DICOMconverter/*.root"
  "DICOMconverter/*.txt"
)
file(COPY ${ROOT_MAP_FILES} DESTINATION ${PROJECT_BINARY_DIR}/maps)

################################################################################
# Find Geant4 package, activating all available UI and Vis drivers by default
################################################################################

option(WITH_GEANT4_UIVIS "RADIONUCLIDE_DOSIMETRY" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

################################################################################
# Setup ROOT Libraries
################################################################################

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})
if (ROOT_FOUND)
  message("ROOT found: ${ROOT_INCLUDE_DIRS}")
else()
   message (FATAL_ERROR "ROOT not found")
endif()

include_directories(${ROOT_INCLUDE_DIRS})

################################################################################
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
################################################################################

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

SET(GCC_COVERAGE_COMPILE_FLAGS "-w")
SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}"  )

################################################################################
# Locate sources and headers for this project
################################################################################

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

################################################################################
# Add the executable, and link it to the Geant4 libraries
################################################################################

add_executable(radionuclide_dosimetry main.cc ${sources} ${headers})
target_link_libraries(radionuclide_dosimetry ${Geant4_LIBRARIES})
target_link_libraries(radionuclide_dosimetry ${ROOT_LIBRARIES})
