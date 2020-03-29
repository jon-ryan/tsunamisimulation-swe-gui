# About this Project
This project was created by students at the Technische Universität München. It implements the shallow water equations (swe)
and adds a GUI on top of it.

# Building the project

## Requirements
- Targetplatform: Linux Desktop
- CMake 3.15 or newer
- Make 
- GTKmm 3 library
- Netcdf library

## Instructions
- Navigate to either of the cmake-build-* folders depending on what kind of build you want.
- Run "make".
- "make clean" is available as well.

## Duplicate files
The file *user_interface.glade* and the image *wave.png* duplicated in the folders cmake-build-* because the
compiled programm requires these files to be in the same directory. The copy of *user_interface.glade* in the
root directory is for development purposes.


