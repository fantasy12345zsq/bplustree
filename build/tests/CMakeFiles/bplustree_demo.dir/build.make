# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/user/Desktop/bplustree

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/user/Desktop/bplustree/build

# Include any dependencies generated for this target.
include tests/CMakeFiles/bplustree_demo.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/bplustree_demo.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/bplustree_demo.dir/flags.make

tests/CMakeFiles/bplustree_demo.dir/bplustree_demo.cc.o: tests/CMakeFiles/bplustree_demo.dir/flags.make
tests/CMakeFiles/bplustree_demo.dir/bplustree_demo.cc.o: ../tests/bplustree_demo.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Desktop/bplustree/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/CMakeFiles/bplustree_demo.dir/bplustree_demo.cc.o"
	cd /home/user/Desktop/bplustree/build/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/bplustree_demo.dir/bplustree_demo.cc.o -c /home/user/Desktop/bplustree/tests/bplustree_demo.cc

tests/CMakeFiles/bplustree_demo.dir/bplustree_demo.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bplustree_demo.dir/bplustree_demo.cc.i"
	cd /home/user/Desktop/bplustree/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/Desktop/bplustree/tests/bplustree_demo.cc > CMakeFiles/bplustree_demo.dir/bplustree_demo.cc.i

tests/CMakeFiles/bplustree_demo.dir/bplustree_demo.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bplustree_demo.dir/bplustree_demo.cc.s"
	cd /home/user/Desktop/bplustree/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/Desktop/bplustree/tests/bplustree_demo.cc -o CMakeFiles/bplustree_demo.dir/bplustree_demo.cc.s

tests/CMakeFiles/bplustree_demo.dir/bplustree_demo.cc.o.requires:

.PHONY : tests/CMakeFiles/bplustree_demo.dir/bplustree_demo.cc.o.requires

tests/CMakeFiles/bplustree_demo.dir/bplustree_demo.cc.o.provides: tests/CMakeFiles/bplustree_demo.dir/bplustree_demo.cc.o.requires
	$(MAKE) -f tests/CMakeFiles/bplustree_demo.dir/build.make tests/CMakeFiles/bplustree_demo.dir/bplustree_demo.cc.o.provides.build
.PHONY : tests/CMakeFiles/bplustree_demo.dir/bplustree_demo.cc.o.provides

tests/CMakeFiles/bplustree_demo.dir/bplustree_demo.cc.o.provides.build: tests/CMakeFiles/bplustree_demo.dir/bplustree_demo.cc.o


# Object files for target bplustree_demo
bplustree_demo_OBJECTS = \
"CMakeFiles/bplustree_demo.dir/bplustree_demo.cc.o"

# External object files for target bplustree_demo
bplustree_demo_EXTERNAL_OBJECTS =

bin/bplustree_demo: tests/CMakeFiles/bplustree_demo.dir/bplustree_demo.cc.o
bin/bplustree_demo: tests/CMakeFiles/bplustree_demo.dir/build.make
bin/bplustree_demo: lib/libbplustree.so.1.0
bin/bplustree_demo: tests/CMakeFiles/bplustree_demo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/user/Desktop/bplustree/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/bplustree_demo"
	cd /home/user/Desktop/bplustree/build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bplustree_demo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/bplustree_demo.dir/build: bin/bplustree_demo

.PHONY : tests/CMakeFiles/bplustree_demo.dir/build

tests/CMakeFiles/bplustree_demo.dir/requires: tests/CMakeFiles/bplustree_demo.dir/bplustree_demo.cc.o.requires

.PHONY : tests/CMakeFiles/bplustree_demo.dir/requires

tests/CMakeFiles/bplustree_demo.dir/clean:
	cd /home/user/Desktop/bplustree/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/bplustree_demo.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/bplustree_demo.dir/clean

tests/CMakeFiles/bplustree_demo.dir/depend:
	cd /home/user/Desktop/bplustree/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/user/Desktop/bplustree /home/user/Desktop/bplustree/tests /home/user/Desktop/bplustree/build /home/user/Desktop/bplustree/build/tests /home/user/Desktop/bplustree/build/tests/CMakeFiles/bplustree_demo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/bplustree_demo.dir/depend

