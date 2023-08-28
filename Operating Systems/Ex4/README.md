# Exercise 4: Virtual Memory Management

This repository contains C++ code for implementing a virtual memory system using paging and page tables. The virtual memory system is designed to manage memory access and mapping between virtual addresses and physical addresses using a hierarchical page table structure.

## Introduction

The virtual memory system provided in `VirtualMemory.h` enables the translation of virtual addresses to physical addresses for reading and writing data. It utilizes a hierarchical page table structure and works in conjunction with a `PhysicalMemory` module to manage memory efficiently.

## Contents

The `VirtualMemory.h` file contains the following components:

- Macros: Definitions for various constants and utility macros used in the implementation.
- `Cyclic` Struct: A struct used to store cyclic operation data.
- `clearTable(uint64_t frameIndex)`: A function to clear a specified frame and set all values to 0.
- `VMinitialize()`: A function to initialize the virtual memory system.
- `separateAddr(uint64_t *arr, uint64_t addr, unsigned int size)`: A function to separate addresses into an array of offsets.
- Memory Traversal Functions: Functions to traverse the page table hierarchy and find free frames for page tables.
- `findFrame(word_t *frame, uint64_t page, word_t lastFrame)`: A function to find a free frame for a page table.
- `getPMAddr(uint64_t virtualAddress)`: A function to convert a virtual address to a physical memory address.
- `VMread(uint64_t virtualAddress, word_t *value)`: A function to read a word from a virtual address.
- `VMwrite(uint64_t virtualAddress, word_t value)`: A function to write a word to a virtual address.

## Usage

1. Include the provided `VirtualMemory.h` file in your project.
2. Use the `VMinitialize()` function to initialize the virtual memory system.
3. Use the `VMread()` function to read a value from a virtual address.
4. Use the `VMwrite()` function to write a value to a virtual address.

Please ensure you have the `PhysicalMemory.h` module available and set up correctly, as the virtual memory system relies on it to access physical memory. Refer to the provided comments and documentation within the code for more details.

## Note

This code is provided for educational purposes and may require additional testing and modifications to suit specific project requirements. Review and adapt the code accordingly before using it in a production environment.
