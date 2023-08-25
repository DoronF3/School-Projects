# Nand-to-Tetris

Welcome to the NAND to Tetris projects! This collection of projects guides you through building a computer system from the ground up, starting with a single NAND gate and culminating in a working computer running Tetris. Each project represents a critical step in understanding and constructing a complete computing system.

## Project 1: HDL Logic Gates and Multiplexers

**Description:** Building logic gates and memory units using hardware description language (HDL).

**Key Files:**
- `And.hdl`: Implements the logic gate AND.
- `Or.hdl`: Implements the OR gate.
- `Not.hdl`: Implements the NOT gate.
- `And16.hdl`: Implements the 16-bit version of the AND gate.
- `Mux.hdl`: Implements a multiplexer.
- `Mux16.hdl`: Implements the 16-bit version of the multiplexer.

![#00FF00](https://via.placeholder.com/15/00FF00/000000?text=+) `HDL`

## Project 2: HDL Arithmetic Logic Unit (ALU) and Arithmetic Components

**Description:** Designing an Arithmetic Logic Unit (ALU) that performs various arithmetic and logic operations.

**Key Files:**
- `ALU.hdl`: Implements the ALU with different arithmetic and logic operations.
- `Add16.hdl`: Implements a 16-bit adder.
- `Mul.hdl`: Implements a basic multiplier.

![#00FF00](https://via.placeholder.com/15/00FF00/000000?text=+) `HDL`

## Project 3: HDL Memory Components

**Description:** Creating different memory units and hierarchical memory systems.

**Key Files:**
- `Bit.hdl`: Implements a single-bit memory cell.
- `RAM8.hdl`: Implements an 8-bit RAM.
- `RAM64.hdl`: Implements a 64-bit RAM.

![#00FF00](https://via.placeholder.com/15/00FF00/000000?text=+) `HDL`

## Project 4: Assembly Language Programs

**Description:** This project involves working with assembly language programming to implement various algorithms and operations.

### Key Files

- `Divide.asm`: Assembly code implementing a division algorithm.
- `Fill.asm`: Assembly code to fill a memory array with specified values.
- `Mult.asm`: Assembly code for a multiplication algorithm.
- `Sort.asm`: Assembly code to sort an array of values.

![#c5f015](https://via.placeholder.com/15/c5f015/000000?text=+) `Assembly`

## Project 5: Computer Architecture

**Description:** Exploring computer architecture by building a simple computer system.

**Key Files:**
- `CPU.hdl`: Implements a basic CPU.
- `Computer.hdl`: Combines the CPU with memory units to create a complete computer.
- `Memory.hdl`: Implements memory components.

![#00FF00](https://via.placeholder.com/15/00FF00/000000?text=+) `HDL`

## Project 6: Assembly Language Tools and Scripts

**Description:** Further development of the assembler to handle more complex assembly instructions.

**Key Files:**
- `Main.py`: Main script for assembling code.
- `Parser.py`: Parses more complex assembly commands.
- `SymbolTable.py`: Manages symbols in the assembly code.

![#1589F0](https://via.placeholder.com/15/1589F0/000000?text=+) `Python` ![#c5f015](https://via.placeholder.com/15/c5f015/000000?text=+) `Assembly`

## Project 7: Virtual Machine I: Stack Arithmetic

**Description:** Building a virtual machine translator for a stack-based computer architecture.

**Key Files:**
- `CodeWriter.py`: Generates assembly code for stack operations.
- `Main.py`: Main script for translating virtual machine code.
- `Parser.py`: Parses virtual machine commands.

![#1589F0](https://via.placeholder.com/15/1589F0/000000?text=+) `Python`

## Project 8: Virtual Machine II: Program Control

**Description:** Expanding the virtual machine translator to handle branching and function calls.

**Key Files:**
- `CodeWriter.py`: Extends code generation to support branching and function call commands.
- `Parser.py`: Parses new virtual machine commands.

![#1589F0](https://via.placeholder.com/15/1589F0/000000?text=+) `Python`

## Project 10: Jack Compiler Part I

**Description:** Building a Jack programming language compiler's front end (tokenizer and syntax analyzer).

**Key Files:**
- `JackTokenizer.py`: Tokenizes Jack source code.
- `CompilationEngine.py`: Parses and generates XML parse trees for Jack code.

![#1589F0](https://via.placeholder.com/15/1589F0/000000?text=+) `Python`

## Project 11: Jack Compiler Part II

**Description:** Completing the Jack programming language compiler by adding a code generator.

**Key Files:**
- `JackCompiler.py`: Orchestrates the compilation process.
- `VMWriter.py`: Generates VM code from the parsed Jack code.

![#1589F0](https://via.placeholder.com/15/1589F0/000000?text=+) `Python`

## Project 12: Jack OS

**Description:** Finalizing the Jack programming language compiler by adding support for classes and subroutines.

**Key Files:**
- `Array.jack`: Implementation of an array class.
- `Keyboard.jack`: Implementation of keyboard input handling.
- `Math.jack`: Mathematical operations implementation.
- `Memory.jack`: Memory access implementation.
- `Output.jack`: Output handling implementation.
- `Screen.jack`: Screen handling implementation.
- `String.jack`: String manipulation implementation.
- `Sys.jack`: System-level operations implementation.

![#FFA500](https://via.placeholder.com/15/FFA500/000000?text=+) `Java`

## Getting Started

1. Navigate into the project subdirectory you are interested in.
2. Explore the provided source code files to learn more about each project.

## Resources

Here are some helpful resources to assist you with the projects:

### Official Course Website
- [NAND to Tetris](https://www.nand2tetris.org/): The official course website with project descriptions, slides, and additional materials.

### Book
- "The Elements of Computing Systems" by Nisan and Schocken: This book accompanies the course and provides in-depth explanations and insights.

### Software Tools
- [Hardware Simulator](https://www.nand2tetris.org/software): A software tool for simulating hardware components and circuits.
- [Assembler](https://www.nand2tetris.org/software): An assembler to translate assembly language programs to machine code.
- [VM Emulator](https://www.nand2tetris.org/software): An emulator for the virtual machine.

### Community and Support
- [Nand2Tetris Forum](https://nand2tetris-questions-and-answers-forum.32033.n3.nabble.com/): A community forum where you can ask questions and interact with others.

### Additional Reading
- [Coursera NAND to Tetris Course](https://www.coursera.org/learn/build-a-computer): A course on Coursera that covers the same material with video lectures and assignments.

These resources will provide you with a comprehensive understanding of the course material and support you throughout the projects.

## License

This collection of HDL files is provided under the [MIT License](LICENSE).
