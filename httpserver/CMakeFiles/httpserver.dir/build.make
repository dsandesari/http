# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.0

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

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
CMAKE_COMMAND = /opt/local/bin/cmake

# The command to remove a file.
RM = /opt/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/ROHITH/http/httpserver

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/ROHITH/http/httpserver

# Include any dependencies generated for this target.
include CMakeFiles/httpserver.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/httpserver.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/httpserver.dir/flags.make

CMakeFiles/httpserver.dir/httpserver.cpp.o: CMakeFiles/httpserver.dir/flags.make
CMakeFiles/httpserver.dir/httpserver.cpp.o: httpserver.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/ROHITH/http/httpserver/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/httpserver.dir/httpserver.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/httpserver.dir/httpserver.cpp.o -c /Users/ROHITH/http/httpserver/httpserver.cpp

CMakeFiles/httpserver.dir/httpserver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/httpserver.dir/httpserver.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/ROHITH/http/httpserver/httpserver.cpp > CMakeFiles/httpserver.dir/httpserver.cpp.i

CMakeFiles/httpserver.dir/httpserver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/httpserver.dir/httpserver.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/ROHITH/http/httpserver/httpserver.cpp -o CMakeFiles/httpserver.dir/httpserver.cpp.s

CMakeFiles/httpserver.dir/httpserver.cpp.o.requires:
.PHONY : CMakeFiles/httpserver.dir/httpserver.cpp.o.requires

CMakeFiles/httpserver.dir/httpserver.cpp.o.provides: CMakeFiles/httpserver.dir/httpserver.cpp.o.requires
	$(MAKE) -f CMakeFiles/httpserver.dir/build.make CMakeFiles/httpserver.dir/httpserver.cpp.o.provides.build
.PHONY : CMakeFiles/httpserver.dir/httpserver.cpp.o.provides

CMakeFiles/httpserver.dir/httpserver.cpp.o.provides.build: CMakeFiles/httpserver.dir/httpserver.cpp.o

# Object files for target httpserver
httpserver_OBJECTS = \
"CMakeFiles/httpserver.dir/httpserver.cpp.o"

# External object files for target httpserver
httpserver_EXTERNAL_OBJECTS =

httpserver: CMakeFiles/httpserver.dir/httpserver.cpp.o
httpserver: CMakeFiles/httpserver.dir/build.make
httpserver: CMakeFiles/httpserver.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable httpserver"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/httpserver.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/httpserver.dir/build: httpserver
.PHONY : CMakeFiles/httpserver.dir/build

CMakeFiles/httpserver.dir/requires: CMakeFiles/httpserver.dir/httpserver.cpp.o.requires
.PHONY : CMakeFiles/httpserver.dir/requires

CMakeFiles/httpserver.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/httpserver.dir/cmake_clean.cmake
.PHONY : CMakeFiles/httpserver.dir/clean

CMakeFiles/httpserver.dir/depend:
	cd /Users/ROHITH/http/httpserver && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/ROHITH/http/httpserver /Users/ROHITH/http/httpserver /Users/ROHITH/http/httpserver /Users/ROHITH/http/httpserver /Users/ROHITH/http/httpserver/CMakeFiles/httpserver.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/httpserver.dir/depend
