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
include lib/CMakeFiles/bplustree.dir/depend.make

# Include the progress variables for this target.
include lib/CMakeFiles/bplustree.dir/progress.make

# Include the compile flags for this target's objects.
include lib/CMakeFiles/bplustree.dir/flags.make

lib/CMakeFiles/bplustree.dir/bplustree.cc.o: lib/CMakeFiles/bplustree.dir/flags.make
lib/CMakeFiles/bplustree.dir/bplustree.cc.o: ../lib/bplustree.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Desktop/bplustree/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object lib/CMakeFiles/bplustree.dir/bplustree.cc.o"
	cd /home/user/Desktop/bplustree/build/lib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/bplustree.dir/bplustree.cc.o -c /home/user/Desktop/bplustree/lib/bplustree.cc

lib/CMakeFiles/bplustree.dir/bplustree.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bplustree.dir/bplustree.cc.i"
	cd /home/user/Desktop/bplustree/build/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/Desktop/bplustree/lib/bplustree.cc > CMakeFiles/bplustree.dir/bplustree.cc.i

lib/CMakeFiles/bplustree.dir/bplustree.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bplustree.dir/bplustree.cc.s"
	cd /home/user/Desktop/bplustree/build/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/Desktop/bplustree/lib/bplustree.cc -o CMakeFiles/bplustree.dir/bplustree.cc.s

lib/CMakeFiles/bplustree.dir/bplustree.cc.o.requires:

.PHONY : lib/CMakeFiles/bplustree.dir/bplustree.cc.o.requires

lib/CMakeFiles/bplustree.dir/bplustree.cc.o.provides: lib/CMakeFiles/bplustree.dir/bplustree.cc.o.requires
	$(MAKE) -f lib/CMakeFiles/bplustree.dir/build.make lib/CMakeFiles/bplustree.dir/bplustree.cc.o.provides.build
.PHONY : lib/CMakeFiles/bplustree.dir/bplustree.cc.o.provides

lib/CMakeFiles/bplustree.dir/bplustree.cc.o.provides.build: lib/CMakeFiles/bplustree.dir/bplustree.cc.o


# Object files for target bplustree
bplustree_OBJECTS = \
"CMakeFiles/bplustree.dir/bplustree.cc.o"

# External object files for target bplustree
bplustree_EXTERNAL_OBJECTS =

lib/libbplustree.so.1.0: lib/CMakeFiles/bplustree.dir/bplustree.cc.o
lib/libbplustree.so.1.0: lib/CMakeFiles/bplustree.dir/build.make
lib/libbplustree.so.1.0: lib/CMakeFiles/bplustree.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/user/Desktop/bplustree/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libbplustree.so"
	cd /home/user/Desktop/bplustree/build/lib && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bplustree.dir/link.txt --verbose=$(VERBOSE)
	cd /home/user/Desktop/bplustree/build/lib && $(CMAKE_COMMAND) -E cmake_symlink_library libbplustree.so.1.0 libbplustree.so.1 libbplustree.so

lib/libbplustree.so.1: lib/libbplustree.so.1.0
	@$(CMAKE_COMMAND) -E touch_nocreate lib/libbplustree.so.1

lib/libbplustree.so: lib/libbplustree.so.1.0
	@$(CMAKE_COMMAND) -E touch_nocreate lib/libbplustree.so

# Rule to build all files generated by this target.
lib/CMakeFiles/bplustree.dir/build: lib/libbplustree.so

.PHONY : lib/CMakeFiles/bplustree.dir/build

lib/CMakeFiles/bplustree.dir/requires: lib/CMakeFiles/bplustree.dir/bplustree.cc.o.requires

.PHONY : lib/CMakeFiles/bplustree.dir/requires

lib/CMakeFiles/bplustree.dir/clean:
	cd /home/user/Desktop/bplustree/build/lib && $(CMAKE_COMMAND) -P CMakeFiles/bplustree.dir/cmake_clean.cmake
.PHONY : lib/CMakeFiles/bplustree.dir/clean

lib/CMakeFiles/bplustree.dir/depend:
	cd /home/user/Desktop/bplustree/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/user/Desktop/bplustree /home/user/Desktop/bplustree/lib /home/user/Desktop/bplustree/build /home/user/Desktop/bplustree/build/lib /home/user/Desktop/bplustree/build/lib/CMakeFiles/bplustree.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/CMakeFiles/bplustree.dir/depend

