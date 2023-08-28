# Exercise 1: OSM Time Measurement

This code provides functions for measuring the time taken by various operations using the OSM (Operating Systems and Multiprogramming) library. The OSM library provides macros for performing specific operations that can be measured to evaluate time performance.

## Overview

The code contains three functions for measuring the time taken by different types of operations:
- `osm_operation_time(unsigned int iterations)`: Measures the time taken to perform a simple arithmetic operation for a specified number of iterations.
- `osm_function_time(unsigned int iterations)`: Measures the time taken to call an empty function for a specified number of iterations.
- `osm_syscall_time(unsigned int iterations)`: Measures the time taken to perform an empty system call for a specified number of iterations.

## Makefile Explanation

The provided Makefile is used to compile and build the code. Here's a breakdown of the Makefile components:

- `CC`, `CXX`: Compiler executable names (g++ in this case).
- `RANLIB`: Command used to generate an index to speed up access to archives.
- `LIBSRC`: Source file containing the OSM library code.
- `LIBOBJ`: Object file(s) generated from LIBSRC.
- `INCS`: Include directories.
- `CFLAGS`, `CXXFLAGS`: Compilation flags.
- `OSMLIB`: Name of the generated static library.
- `TARGETS`: List of targets to build.
- `TAR`, `TARFLAGS`, `TARNAME`, `TARSRCS`: Variables for creating a tar archive.

Makefile Rules:
- `all`: The default target that builds the static library (OSMLIB).
- `$(TARGETS)`: Builds the static library using LIBOBJ.
- `clean`: Removes generated files and temporary files.
- `depend`: Generates dependencies for source files.
- `tar`: Creates a tar archive of the source code and other relevant files.

## Usage

1. Include the required headers and write your code as described in the previous sections.

```cpp
#include "osm.h"
#include <sys/time.h>
#include <iostream>
```

2. Use the provided functions to measure the time for different types of operations:

```cpp
unsigned int iterations = 1000000; // Number of iterations for measurement

double operationTime = osm_operation_time(iterations);
double functionTime = osm_function_time(iterations);
double syscallTime = osm_syscall_time(iterations);

std::cout << "Time for arithmetic operation: " << operationTime << " nanoseconds" << std::endl;
std::cout << "Time for function call: " << functionTime << " nanoseconds" << std::endl;
std::cout << "Time for system call: " << syscallTime << " nanoseconds" << std::endl;
```

3. Open a terminal in the project directory and use the following commands:
   - `make`: Builds the static library.
   - `make clean`: Removes generated files.
   - `make depend`: Generates dependencies for source files.
   - `make tar`: Creates a tar archive of the source code and other files.

## Constants

- `SECOND_TO_NANO`: Conversion factor from seconds to nanoseconds.
- `MICRO_TO_NANO`: Conversion factor from microseconds to nanoseconds.

## Notes

- The provided functions measure the average time taken per iteration for the specified operation. The result is in nanoseconds.
- The OSM library's `OSM_NULLSYSCALL` macro is used to perform an empty system call.

## License

This code is provided under the [MIT License](LICENSE).

Feel free to use and modify this code for your own purposes.
