# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

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
CMAKE_SOURCE_DIR = /home/frodo/Gradschool/Lexer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/frodo/Gradschool/Lexer/build

# Include any dependencies generated for this target.
include CMakeFiles/mc.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/mc.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/mc.dir/flags.make

CMakeFiles/mc.dir/main.cpp.o: CMakeFiles/mc.dir/flags.make
CMakeFiles/mc.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/frodo/Gradschool/Lexer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/mc.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mc.dir/main.cpp.o -c /home/frodo/Gradschool/Lexer/main.cpp

CMakeFiles/mc.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mc.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/frodo/Gradschool/Lexer/main.cpp > CMakeFiles/mc.dir/main.cpp.i

CMakeFiles/mc.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mc.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/frodo/Gradschool/Lexer/main.cpp -o CMakeFiles/mc.dir/main.cpp.s

CMakeFiles/mc.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/mc.dir/main.cpp.o.requires

CMakeFiles/mc.dir/main.cpp.o.provides: CMakeFiles/mc.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/mc.dir/build.make CMakeFiles/mc.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/mc.dir/main.cpp.o.provides

CMakeFiles/mc.dir/main.cpp.o.provides.build: CMakeFiles/mc.dir/main.cpp.o


CMakeFiles/mc.dir/Lexer.cpp.o: CMakeFiles/mc.dir/flags.make
CMakeFiles/mc.dir/Lexer.cpp.o: ../Lexer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/frodo/Gradschool/Lexer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/mc.dir/Lexer.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mc.dir/Lexer.cpp.o -c /home/frodo/Gradschool/Lexer/Lexer.cpp

CMakeFiles/mc.dir/Lexer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mc.dir/Lexer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/frodo/Gradschool/Lexer/Lexer.cpp > CMakeFiles/mc.dir/Lexer.cpp.i

CMakeFiles/mc.dir/Lexer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mc.dir/Lexer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/frodo/Gradschool/Lexer/Lexer.cpp -o CMakeFiles/mc.dir/Lexer.cpp.s

CMakeFiles/mc.dir/Lexer.cpp.o.requires:

.PHONY : CMakeFiles/mc.dir/Lexer.cpp.o.requires

CMakeFiles/mc.dir/Lexer.cpp.o.provides: CMakeFiles/mc.dir/Lexer.cpp.o.requires
	$(MAKE) -f CMakeFiles/mc.dir/build.make CMakeFiles/mc.dir/Lexer.cpp.o.provides.build
.PHONY : CMakeFiles/mc.dir/Lexer.cpp.o.provides

CMakeFiles/mc.dir/Lexer.cpp.o.provides.build: CMakeFiles/mc.dir/Lexer.cpp.o


CMakeFiles/mc.dir/Token.cpp.o: CMakeFiles/mc.dir/flags.make
CMakeFiles/mc.dir/Token.cpp.o: ../Token.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/frodo/Gradschool/Lexer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/mc.dir/Token.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mc.dir/Token.cpp.o -c /home/frodo/Gradschool/Lexer/Token.cpp

CMakeFiles/mc.dir/Token.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mc.dir/Token.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/frodo/Gradschool/Lexer/Token.cpp > CMakeFiles/mc.dir/Token.cpp.i

CMakeFiles/mc.dir/Token.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mc.dir/Token.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/frodo/Gradschool/Lexer/Token.cpp -o CMakeFiles/mc.dir/Token.cpp.s

CMakeFiles/mc.dir/Token.cpp.o.requires:

.PHONY : CMakeFiles/mc.dir/Token.cpp.o.requires

CMakeFiles/mc.dir/Token.cpp.o.provides: CMakeFiles/mc.dir/Token.cpp.o.requires
	$(MAKE) -f CMakeFiles/mc.dir/build.make CMakeFiles/mc.dir/Token.cpp.o.provides.build
.PHONY : CMakeFiles/mc.dir/Token.cpp.o.provides

CMakeFiles/mc.dir/Token.cpp.o.provides.build: CMakeFiles/mc.dir/Token.cpp.o


# Object files for target mc
mc_OBJECTS = \
"CMakeFiles/mc.dir/main.cpp.o" \
"CMakeFiles/mc.dir/Lexer.cpp.o" \
"CMakeFiles/mc.dir/Token.cpp.o"

# External object files for target mc
mc_EXTERNAL_OBJECTS =

mc: CMakeFiles/mc.dir/main.cpp.o
mc: CMakeFiles/mc.dir/Lexer.cpp.o
mc: CMakeFiles/mc.dir/Token.cpp.o
mc: CMakeFiles/mc.dir/build.make
mc: CMakeFiles/mc.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/frodo/Gradschool/Lexer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable mc"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mc.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/mc.dir/build: mc

.PHONY : CMakeFiles/mc.dir/build

CMakeFiles/mc.dir/requires: CMakeFiles/mc.dir/main.cpp.o.requires
CMakeFiles/mc.dir/requires: CMakeFiles/mc.dir/Lexer.cpp.o.requires
CMakeFiles/mc.dir/requires: CMakeFiles/mc.dir/Token.cpp.o.requires

.PHONY : CMakeFiles/mc.dir/requires

CMakeFiles/mc.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/mc.dir/cmake_clean.cmake
.PHONY : CMakeFiles/mc.dir/clean

CMakeFiles/mc.dir/depend:
	cd /home/frodo/Gradschool/Lexer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/frodo/Gradschool/Lexer /home/frodo/Gradschool/Lexer /home/frodo/Gradschool/Lexer/build /home/frodo/Gradschool/Lexer/build /home/frodo/Gradschool/Lexer/build/CMakeFiles/mc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/mc.dir/depend
