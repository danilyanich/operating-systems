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
CMAKE_SOURCE_DIR = /home/elizabeth/OS-4

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/elizabeth/OS-4

# Include any dependencies generated for this target.
include CMakeFiles/FileSystem.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/FileSystem.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/FileSystem.dir/flags.make

CMakeFiles/FileSystem.dir/main.cpp.o: CMakeFiles/FileSystem.dir/flags.make
CMakeFiles/FileSystem.dir/main.cpp.o: main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/elizabeth/OS-4/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/FileSystem.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FileSystem.dir/main.cpp.o -c /home/elizabeth/OS-4/main.cpp

CMakeFiles/FileSystem.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FileSystem.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/elizabeth/OS-4/main.cpp > CMakeFiles/FileSystem.dir/main.cpp.i

CMakeFiles/FileSystem.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FileSystem.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/elizabeth/OS-4/main.cpp -o CMakeFiles/FileSystem.dir/main.cpp.s

CMakeFiles/FileSystem.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/FileSystem.dir/main.cpp.o.requires

CMakeFiles/FileSystem.dir/main.cpp.o.provides: CMakeFiles/FileSystem.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/FileSystem.dir/build.make CMakeFiles/FileSystem.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/FileSystem.dir/main.cpp.o.provides

CMakeFiles/FileSystem.dir/main.cpp.o.provides.build: CMakeFiles/FileSystem.dir/main.cpp.o


CMakeFiles/FileSystem.dir/command.cpp.o: CMakeFiles/FileSystem.dir/flags.make
CMakeFiles/FileSystem.dir/command.cpp.o: command.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/elizabeth/OS-4/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/FileSystem.dir/command.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FileSystem.dir/command.cpp.o -c /home/elizabeth/OS-4/command.cpp

CMakeFiles/FileSystem.dir/command.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FileSystem.dir/command.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/elizabeth/OS-4/command.cpp > CMakeFiles/FileSystem.dir/command.cpp.i

CMakeFiles/FileSystem.dir/command.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FileSystem.dir/command.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/elizabeth/OS-4/command.cpp -o CMakeFiles/FileSystem.dir/command.cpp.s

CMakeFiles/FileSystem.dir/command.cpp.o.requires:

.PHONY : CMakeFiles/FileSystem.dir/command.cpp.o.requires

CMakeFiles/FileSystem.dir/command.cpp.o.provides: CMakeFiles/FileSystem.dir/command.cpp.o.requires
	$(MAKE) -f CMakeFiles/FileSystem.dir/build.make CMakeFiles/FileSystem.dir/command.cpp.o.provides.build
.PHONY : CMakeFiles/FileSystem.dir/command.cpp.o.provides

CMakeFiles/FileSystem.dir/command.cpp.o.provides.build: CMakeFiles/FileSystem.dir/command.cpp.o


CMakeFiles/FileSystem.dir/init.cpp.o: CMakeFiles/FileSystem.dir/flags.make
CMakeFiles/FileSystem.dir/init.cpp.o: init.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/elizabeth/OS-4/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/FileSystem.dir/init.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FileSystem.dir/init.cpp.o -c /home/elizabeth/OS-4/init.cpp

CMakeFiles/FileSystem.dir/init.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FileSystem.dir/init.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/elizabeth/OS-4/init.cpp > CMakeFiles/FileSystem.dir/init.cpp.i

CMakeFiles/FileSystem.dir/init.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FileSystem.dir/init.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/elizabeth/OS-4/init.cpp -o CMakeFiles/FileSystem.dir/init.cpp.s

CMakeFiles/FileSystem.dir/init.cpp.o.requires:

.PHONY : CMakeFiles/FileSystem.dir/init.cpp.o.requires

CMakeFiles/FileSystem.dir/init.cpp.o.provides: CMakeFiles/FileSystem.dir/init.cpp.o.requires
	$(MAKE) -f CMakeFiles/FileSystem.dir/build.make CMakeFiles/FileSystem.dir/init.cpp.o.provides.build
.PHONY : CMakeFiles/FileSystem.dir/init.cpp.o.provides

CMakeFiles/FileSystem.dir/init.cpp.o.provides.build: CMakeFiles/FileSystem.dir/init.cpp.o


CMakeFiles/FileSystem.dir/load.cpp.o: CMakeFiles/FileSystem.dir/flags.make
CMakeFiles/FileSystem.dir/load.cpp.o: load.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/elizabeth/OS-4/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/FileSystem.dir/load.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FileSystem.dir/load.cpp.o -c /home/elizabeth/OS-4/load.cpp

CMakeFiles/FileSystem.dir/load.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FileSystem.dir/load.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/elizabeth/OS-4/load.cpp > CMakeFiles/FileSystem.dir/load.cpp.i

CMakeFiles/FileSystem.dir/load.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FileSystem.dir/load.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/elizabeth/OS-4/load.cpp -o CMakeFiles/FileSystem.dir/load.cpp.s

CMakeFiles/FileSystem.dir/load.cpp.o.requires:

.PHONY : CMakeFiles/FileSystem.dir/load.cpp.o.requires

CMakeFiles/FileSystem.dir/load.cpp.o.provides: CMakeFiles/FileSystem.dir/load.cpp.o.requires
	$(MAKE) -f CMakeFiles/FileSystem.dir/build.make CMakeFiles/FileSystem.dir/load.cpp.o.provides.build
.PHONY : CMakeFiles/FileSystem.dir/load.cpp.o.provides

CMakeFiles/FileSystem.dir/load.cpp.o.provides.build: CMakeFiles/FileSystem.dir/load.cpp.o


CMakeFiles/FileSystem.dir/filecommand.cpp.o: CMakeFiles/FileSystem.dir/flags.make
CMakeFiles/FileSystem.dir/filecommand.cpp.o: filecommand.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/elizabeth/OS-4/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/FileSystem.dir/filecommand.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FileSystem.dir/filecommand.cpp.o -c /home/elizabeth/OS-4/filecommand.cpp

CMakeFiles/FileSystem.dir/filecommand.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FileSystem.dir/filecommand.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/elizabeth/OS-4/filecommand.cpp > CMakeFiles/FileSystem.dir/filecommand.cpp.i

CMakeFiles/FileSystem.dir/filecommand.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FileSystem.dir/filecommand.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/elizabeth/OS-4/filecommand.cpp -o CMakeFiles/FileSystem.dir/filecommand.cpp.s

CMakeFiles/FileSystem.dir/filecommand.cpp.o.requires:

.PHONY : CMakeFiles/FileSystem.dir/filecommand.cpp.o.requires

CMakeFiles/FileSystem.dir/filecommand.cpp.o.provides: CMakeFiles/FileSystem.dir/filecommand.cpp.o.requires
	$(MAKE) -f CMakeFiles/FileSystem.dir/build.make CMakeFiles/FileSystem.dir/filecommand.cpp.o.provides.build
.PHONY : CMakeFiles/FileSystem.dir/filecommand.cpp.o.provides

CMakeFiles/FileSystem.dir/filecommand.cpp.o.provides.build: CMakeFiles/FileSystem.dir/filecommand.cpp.o


CMakeFiles/FileSystem.dir/commonfunc.cpp.o: CMakeFiles/FileSystem.dir/flags.make
CMakeFiles/FileSystem.dir/commonfunc.cpp.o: commonfunc.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/elizabeth/OS-4/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/FileSystem.dir/commonfunc.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FileSystem.dir/commonfunc.cpp.o -c /home/elizabeth/OS-4/commonfunc.cpp

CMakeFiles/FileSystem.dir/commonfunc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FileSystem.dir/commonfunc.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/elizabeth/OS-4/commonfunc.cpp > CMakeFiles/FileSystem.dir/commonfunc.cpp.i

CMakeFiles/FileSystem.dir/commonfunc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FileSystem.dir/commonfunc.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/elizabeth/OS-4/commonfunc.cpp -o CMakeFiles/FileSystem.dir/commonfunc.cpp.s

CMakeFiles/FileSystem.dir/commonfunc.cpp.o.requires:

.PHONY : CMakeFiles/FileSystem.dir/commonfunc.cpp.o.requires

CMakeFiles/FileSystem.dir/commonfunc.cpp.o.provides: CMakeFiles/FileSystem.dir/commonfunc.cpp.o.requires
	$(MAKE) -f CMakeFiles/FileSystem.dir/build.make CMakeFiles/FileSystem.dir/commonfunc.cpp.o.provides.build
.PHONY : CMakeFiles/FileSystem.dir/commonfunc.cpp.o.provides

CMakeFiles/FileSystem.dir/commonfunc.cpp.o.provides.build: CMakeFiles/FileSystem.dir/commonfunc.cpp.o


# Object files for target FileSystem
FileSystem_OBJECTS = \
"CMakeFiles/FileSystem.dir/main.cpp.o" \
"CMakeFiles/FileSystem.dir/command.cpp.o" \
"CMakeFiles/FileSystem.dir/init.cpp.o" \
"CMakeFiles/FileSystem.dir/load.cpp.o" \
"CMakeFiles/FileSystem.dir/filecommand.cpp.o" \
"CMakeFiles/FileSystem.dir/commonfunc.cpp.o"

# External object files for target FileSystem
FileSystem_EXTERNAL_OBJECTS =

FileSystem: CMakeFiles/FileSystem.dir/main.cpp.o
FileSystem: CMakeFiles/FileSystem.dir/command.cpp.o
FileSystem: CMakeFiles/FileSystem.dir/init.cpp.o
FileSystem: CMakeFiles/FileSystem.dir/load.cpp.o
FileSystem: CMakeFiles/FileSystem.dir/filecommand.cpp.o
FileSystem: CMakeFiles/FileSystem.dir/commonfunc.cpp.o
FileSystem: CMakeFiles/FileSystem.dir/build.make
FileSystem: CMakeFiles/FileSystem.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/elizabeth/OS-4/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable FileSystem"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/FileSystem.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/FileSystem.dir/build: FileSystem

.PHONY : CMakeFiles/FileSystem.dir/build

CMakeFiles/FileSystem.dir/requires: CMakeFiles/FileSystem.dir/main.cpp.o.requires
CMakeFiles/FileSystem.dir/requires: CMakeFiles/FileSystem.dir/command.cpp.o.requires
CMakeFiles/FileSystem.dir/requires: CMakeFiles/FileSystem.dir/init.cpp.o.requires
CMakeFiles/FileSystem.dir/requires: CMakeFiles/FileSystem.dir/load.cpp.o.requires
CMakeFiles/FileSystem.dir/requires: CMakeFiles/FileSystem.dir/filecommand.cpp.o.requires
CMakeFiles/FileSystem.dir/requires: CMakeFiles/FileSystem.dir/commonfunc.cpp.o.requires

.PHONY : CMakeFiles/FileSystem.dir/requires

CMakeFiles/FileSystem.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/FileSystem.dir/cmake_clean.cmake
.PHONY : CMakeFiles/FileSystem.dir/clean

CMakeFiles/FileSystem.dir/depend:
	cd /home/elizabeth/OS-4 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/elizabeth/OS-4 /home/elizabeth/OS-4 /home/elizabeth/OS-4 /home/elizabeth/OS-4 /home/elizabeth/OS-4/CMakeFiles/FileSystem.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/FileSystem.dir/depend
