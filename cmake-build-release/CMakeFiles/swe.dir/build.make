# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Produce verbose output by default.
VERBOSE = 1

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/clion-2019.3.2/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /opt/clion-2019.3.2/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jherrmann/Documents/Tsunamisimulation/swe-gui

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jherrmann/Documents/Tsunamisimulation/swe-gui/cmake-build-release

# Include any dependencies generated for this target.
include CMakeFiles/swe.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/swe.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/swe.dir/flags.make

CMakeFiles/swe.dir/src/main.cpp.o: CMakeFiles/swe.dir/flags.make
CMakeFiles/swe.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jherrmann/Documents/Tsunamisimulation/swe-gui/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/swe.dir/src/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/swe.dir/src/main.cpp.o -c /home/jherrmann/Documents/Tsunamisimulation/swe-gui/src/main.cpp

CMakeFiles/swe.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/swe.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jherrmann/Documents/Tsunamisimulation/swe-gui/src/main.cpp > CMakeFiles/swe.dir/src/main.cpp.i

CMakeFiles/swe.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/swe.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jherrmann/Documents/Tsunamisimulation/swe-gui/src/main.cpp -o CMakeFiles/swe.dir/src/main.cpp.s

CMakeFiles/swe.dir/src/gui.cpp.o: CMakeFiles/swe.dir/flags.make
CMakeFiles/swe.dir/src/gui.cpp.o: ../src/gui.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jherrmann/Documents/Tsunamisimulation/swe-gui/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/swe.dir/src/gui.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/swe.dir/src/gui.cpp.o -c /home/jherrmann/Documents/Tsunamisimulation/swe-gui/src/gui.cpp

CMakeFiles/swe.dir/src/gui.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/swe.dir/src/gui.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jherrmann/Documents/Tsunamisimulation/swe-gui/src/gui.cpp > CMakeFiles/swe.dir/src/gui.cpp.i

CMakeFiles/swe.dir/src/gui.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/swe.dir/src/gui.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jherrmann/Documents/Tsunamisimulation/swe-gui/src/gui.cpp -o CMakeFiles/swe.dir/src/gui.cpp.s

CMakeFiles/swe.dir/src/scenario.cpp.o: CMakeFiles/swe.dir/flags.make
CMakeFiles/swe.dir/src/scenario.cpp.o: ../src/scenario.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jherrmann/Documents/Tsunamisimulation/swe-gui/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/swe.dir/src/scenario.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/swe.dir/src/scenario.cpp.o -c /home/jherrmann/Documents/Tsunamisimulation/swe-gui/src/scenario.cpp

CMakeFiles/swe.dir/src/scenario.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/swe.dir/src/scenario.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jherrmann/Documents/Tsunamisimulation/swe-gui/src/scenario.cpp > CMakeFiles/swe.dir/src/scenario.cpp.i

CMakeFiles/swe.dir/src/scenario.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/swe.dir/src/scenario.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jherrmann/Documents/Tsunamisimulation/swe-gui/src/scenario.cpp -o CMakeFiles/swe.dir/src/scenario.cpp.s

CMakeFiles/swe.dir/src/simulation.cpp.o: CMakeFiles/swe.dir/flags.make
CMakeFiles/swe.dir/src/simulation.cpp.o: ../src/simulation.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jherrmann/Documents/Tsunamisimulation/swe-gui/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/swe.dir/src/simulation.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/swe.dir/src/simulation.cpp.o -c /home/jherrmann/Documents/Tsunamisimulation/swe-gui/src/simulation.cpp

CMakeFiles/swe.dir/src/simulation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/swe.dir/src/simulation.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jherrmann/Documents/Tsunamisimulation/swe-gui/src/simulation.cpp > CMakeFiles/swe.dir/src/simulation.cpp.i

CMakeFiles/swe.dir/src/simulation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/swe.dir/src/simulation.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jherrmann/Documents/Tsunamisimulation/swe-gui/src/simulation.cpp -o CMakeFiles/swe.dir/src/simulation.cpp.s

# Object files for target swe
swe_OBJECTS = \
"CMakeFiles/swe.dir/src/main.cpp.o" \
"CMakeFiles/swe.dir/src/gui.cpp.o" \
"CMakeFiles/swe.dir/src/scenario.cpp.o" \
"CMakeFiles/swe.dir/src/simulation.cpp.o"

# External object files for target swe
swe_EXTERNAL_OBJECTS =

swe: CMakeFiles/swe.dir/src/main.cpp.o
swe: CMakeFiles/swe.dir/src/gui.cpp.o
swe: CMakeFiles/swe.dir/src/scenario.cpp.o
swe: CMakeFiles/swe.dir/src/simulation.cpp.o
swe: CMakeFiles/swe.dir/build.make
swe: /usr/lib/libnetcdf_c++4.so
swe: /usr/lib/libnetcdf.so
swe: /usr/lib/libgtkmm-3.0.so
swe: /usr/lib/libatkmm-1.6.so
swe: /usr/lib/libgdkmm-3.0.so
swe: /usr/lib/libgiomm-2.4.so
swe: /usr/lib/libpangomm-1.4.so
swe: /usr/lib/libglibmm-2.4.so
swe: /usr/lib/libgtk-3.so
swe: /usr/lib/libgdk-3.so
swe: /usr/lib/libz.so
swe: /usr/lib/libpangocairo-1.0.so
swe: /usr/lib/libpango-1.0.so
swe: /usr/lib/libharfbuzz.so
swe: /usr/lib/libatk-1.0.so
swe: /usr/lib/libcairo-gobject.so
swe: /usr/lib/libgio-2.0.so
swe: /usr/lib/libcairomm-1.0.so
swe: /usr/lib/libcairo.so
swe: /usr/lib/libsigc-2.0.so
swe: /usr/lib/libgdk_pixbuf-2.0.so
swe: /usr/lib/libgobject-2.0.so
swe: /usr/lib/libglib-2.0.so
swe: /usr/lib/libgomp.so
swe: /usr/lib/libpthread.so
swe: CMakeFiles/swe.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jherrmann/Documents/Tsunamisimulation/swe-gui/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable swe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/swe.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/swe.dir/build: swe

.PHONY : CMakeFiles/swe.dir/build

CMakeFiles/swe.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/swe.dir/cmake_clean.cmake
.PHONY : CMakeFiles/swe.dir/clean

CMakeFiles/swe.dir/depend:
	cd /home/jherrmann/Documents/Tsunamisimulation/swe-gui/cmake-build-release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jherrmann/Documents/Tsunamisimulation/swe-gui /home/jherrmann/Documents/Tsunamisimulation/swe-gui /home/jherrmann/Documents/Tsunamisimulation/swe-gui/cmake-build-release /home/jherrmann/Documents/Tsunamisimulation/swe-gui/cmake-build-release /home/jherrmann/Documents/Tsunamisimulation/swe-gui/cmake-build-release/CMakeFiles/swe.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/swe.dir/depend

