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
CMAKE_SOURCE_DIR = /homex/student/Desktop/SK-C/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /homex/student/Desktop/SK-C

# Include any dependencies generated for this target.
include CMakeFiles/SK.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/SK.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/SK.dir/flags.make

CMakeFiles/SK.dir/main.cpp.o: CMakeFiles/SK.dir/flags.make
CMakeFiles/SK.dir/main.cpp.o: src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/homex/student/Desktop/SK-C/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/SK.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SK.dir/main.cpp.o -c /homex/student/Desktop/SK-C/src/main.cpp

CMakeFiles/SK.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SK.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /homex/student/Desktop/SK-C/src/main.cpp > CMakeFiles/SK.dir/main.cpp.i

CMakeFiles/SK.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SK.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /homex/student/Desktop/SK-C/src/main.cpp -o CMakeFiles/SK.dir/main.cpp.s

CMakeFiles/SK.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/SK.dir/main.cpp.o.requires

CMakeFiles/SK.dir/main.cpp.o.provides: CMakeFiles/SK.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/SK.dir/build.make CMakeFiles/SK.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/SK.dir/main.cpp.o.provides

CMakeFiles/SK.dir/main.cpp.o.provides.build: CMakeFiles/SK.dir/main.cpp.o


CMakeFiles/SK.dir/Commons.cpp.o: CMakeFiles/SK.dir/flags.make
CMakeFiles/SK.dir/Commons.cpp.o: src/Commons.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/homex/student/Desktop/SK-C/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/SK.dir/Commons.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SK.dir/Commons.cpp.o -c /homex/student/Desktop/SK-C/src/Commons.cpp

CMakeFiles/SK.dir/Commons.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SK.dir/Commons.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /homex/student/Desktop/SK-C/src/Commons.cpp > CMakeFiles/SK.dir/Commons.cpp.i

CMakeFiles/SK.dir/Commons.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SK.dir/Commons.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /homex/student/Desktop/SK-C/src/Commons.cpp -o CMakeFiles/SK.dir/Commons.cpp.s

CMakeFiles/SK.dir/Commons.cpp.o.requires:

.PHONY : CMakeFiles/SK.dir/Commons.cpp.o.requires

CMakeFiles/SK.dir/Commons.cpp.o.provides: CMakeFiles/SK.dir/Commons.cpp.o.requires
	$(MAKE) -f CMakeFiles/SK.dir/build.make CMakeFiles/SK.dir/Commons.cpp.o.provides.build
.PHONY : CMakeFiles/SK.dir/Commons.cpp.o.provides

CMakeFiles/SK.dir/Commons.cpp.o.provides.build: CMakeFiles/SK.dir/Commons.cpp.o


CMakeFiles/SK.dir/messageTools.cpp.o: CMakeFiles/SK.dir/flags.make
CMakeFiles/SK.dir/messageTools.cpp.o: src/messageTools.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/homex/student/Desktop/SK-C/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/SK.dir/messageTools.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SK.dir/messageTools.cpp.o -c /homex/student/Desktop/SK-C/src/messageTools.cpp

CMakeFiles/SK.dir/messageTools.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SK.dir/messageTools.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /homex/student/Desktop/SK-C/src/messageTools.cpp > CMakeFiles/SK.dir/messageTools.cpp.i

CMakeFiles/SK.dir/messageTools.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SK.dir/messageTools.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /homex/student/Desktop/SK-C/src/messageTools.cpp -o CMakeFiles/SK.dir/messageTools.cpp.s

CMakeFiles/SK.dir/messageTools.cpp.o.requires:

.PHONY : CMakeFiles/SK.dir/messageTools.cpp.o.requires

CMakeFiles/SK.dir/messageTools.cpp.o.provides: CMakeFiles/SK.dir/messageTools.cpp.o.requires
	$(MAKE) -f CMakeFiles/SK.dir/build.make CMakeFiles/SK.dir/messageTools.cpp.o.provides.build
.PHONY : CMakeFiles/SK.dir/messageTools.cpp.o.provides

CMakeFiles/SK.dir/messageTools.cpp.o.provides.build: CMakeFiles/SK.dir/messageTools.cpp.o


CMakeFiles/SK.dir/connectionHandler.cpp.o: CMakeFiles/SK.dir/flags.make
CMakeFiles/SK.dir/connectionHandler.cpp.o: src/connectionHandler.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/homex/student/Desktop/SK-C/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/SK.dir/connectionHandler.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SK.dir/connectionHandler.cpp.o -c /homex/student/Desktop/SK-C/src/connectionHandler.cpp

CMakeFiles/SK.dir/connectionHandler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SK.dir/connectionHandler.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /homex/student/Desktop/SK-C/src/connectionHandler.cpp > CMakeFiles/SK.dir/connectionHandler.cpp.i

CMakeFiles/SK.dir/connectionHandler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SK.dir/connectionHandler.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /homex/student/Desktop/SK-C/src/connectionHandler.cpp -o CMakeFiles/SK.dir/connectionHandler.cpp.s

CMakeFiles/SK.dir/connectionHandler.cpp.o.requires:

.PHONY : CMakeFiles/SK.dir/connectionHandler.cpp.o.requires

CMakeFiles/SK.dir/connectionHandler.cpp.o.provides: CMakeFiles/SK.dir/connectionHandler.cpp.o.requires
	$(MAKE) -f CMakeFiles/SK.dir/build.make CMakeFiles/SK.dir/connectionHandler.cpp.o.provides.build
.PHONY : CMakeFiles/SK.dir/connectionHandler.cpp.o.provides

CMakeFiles/SK.dir/connectionHandler.cpp.o.provides.build: CMakeFiles/SK.dir/connectionHandler.cpp.o


CMakeFiles/SK.dir/connectionFunctions.cpp.o: CMakeFiles/SK.dir/flags.make
CMakeFiles/SK.dir/connectionFunctions.cpp.o: src/connectionFunctions.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/homex/student/Desktop/SK-C/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/SK.dir/connectionFunctions.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SK.dir/connectionFunctions.cpp.o -c /homex/student/Desktop/SK-C/src/connectionFunctions.cpp

CMakeFiles/SK.dir/connectionFunctions.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SK.dir/connectionFunctions.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /homex/student/Desktop/SK-C/src/connectionFunctions.cpp > CMakeFiles/SK.dir/connectionFunctions.cpp.i

CMakeFiles/SK.dir/connectionFunctions.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SK.dir/connectionFunctions.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /homex/student/Desktop/SK-C/src/connectionFunctions.cpp -o CMakeFiles/SK.dir/connectionFunctions.cpp.s

CMakeFiles/SK.dir/connectionFunctions.cpp.o.requires:

.PHONY : CMakeFiles/SK.dir/connectionFunctions.cpp.o.requires

CMakeFiles/SK.dir/connectionFunctions.cpp.o.provides: CMakeFiles/SK.dir/connectionFunctions.cpp.o.requires
	$(MAKE) -f CMakeFiles/SK.dir/build.make CMakeFiles/SK.dir/connectionFunctions.cpp.o.provides.build
.PHONY : CMakeFiles/SK.dir/connectionFunctions.cpp.o.provides

CMakeFiles/SK.dir/connectionFunctions.cpp.o.provides.build: CMakeFiles/SK.dir/connectionFunctions.cpp.o


# Object files for target SK
SK_OBJECTS = \
"CMakeFiles/SK.dir/main.cpp.o" \
"CMakeFiles/SK.dir/Commons.cpp.o" \
"CMakeFiles/SK.dir/messageTools.cpp.o" \
"CMakeFiles/SK.dir/connectionHandler.cpp.o" \
"CMakeFiles/SK.dir/connectionFunctions.cpp.o"

# External object files for target SK
SK_EXTERNAL_OBJECTS =

SK: CMakeFiles/SK.dir/main.cpp.o
SK: CMakeFiles/SK.dir/Commons.cpp.o
SK: CMakeFiles/SK.dir/messageTools.cpp.o
SK: CMakeFiles/SK.dir/connectionHandler.cpp.o
SK: CMakeFiles/SK.dir/connectionFunctions.cpp.o
SK: CMakeFiles/SK.dir/build.make
SK: CMakeFiles/SK.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/homex/student/Desktop/SK-C/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable SK"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SK.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/SK.dir/build: SK

.PHONY : CMakeFiles/SK.dir/build

CMakeFiles/SK.dir/requires: CMakeFiles/SK.dir/main.cpp.o.requires
CMakeFiles/SK.dir/requires: CMakeFiles/SK.dir/Commons.cpp.o.requires
CMakeFiles/SK.dir/requires: CMakeFiles/SK.dir/messageTools.cpp.o.requires
CMakeFiles/SK.dir/requires: CMakeFiles/SK.dir/connectionHandler.cpp.o.requires
CMakeFiles/SK.dir/requires: CMakeFiles/SK.dir/connectionFunctions.cpp.o.requires

.PHONY : CMakeFiles/SK.dir/requires

CMakeFiles/SK.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/SK.dir/cmake_clean.cmake
.PHONY : CMakeFiles/SK.dir/clean

CMakeFiles/SK.dir/depend:
	cd /homex/student/Desktop/SK-C && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /homex/student/Desktop/SK-C/src /homex/student/Desktop/SK-C/src /homex/student/Desktop/SK-C /homex/student/Desktop/SK-C /homex/student/Desktop/SK-C/CMakeFiles/SK.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/SK.dir/depend
