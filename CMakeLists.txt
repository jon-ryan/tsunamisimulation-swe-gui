# Clion ships with version 3.15.3 at time of writing
cmake_minimum_required(VERSION 3.15)

# Project name and language
project(SWE-GUI CXX)

# We use C++17 without any extensions
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_VERBOSE_MAKEFILE ON)

# Initialize the "pkg_check_modules" function
find_package(PkgConfig REQUIRED)

# Get netCDF-4 C++ data
pkg_check_modules(NETCDF REQUIRED IMPORTED_TARGET netcdf-cxx4)

# Get gtkmm 3.0 data
pkg_check_modules(GTKMM REQUIRED IMPORTED_TARGET gtkmm-3.0)

# Get OpenMP data
find_package(OpenMP REQUIRED)

# Main executable
add_executable(swe src/main.cpp src/gui.cpp src/scenario.cpp  src/simulation.cpp src/solver.h)

# Link libraries
target_link_libraries(swe PkgConfig::NETCDF PkgConfig::GTKMM OpenMP::OpenMP_CXX)
