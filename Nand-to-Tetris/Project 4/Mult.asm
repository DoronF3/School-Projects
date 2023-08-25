// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

        @R2
        M=0     // Initialize R2 to 0
        @total
        M=0     // Initialize total to 0
        @R1
        D=M
        @n
        M=D     // Initialize n to the value at RAM[1]
(SUM)
        @n
        D=M
        @END
        D;JEQ   // Check if n is 0, if so, goto END
        @R0
        D=M
        @total
        M=M+D   // Add RAM[0] to total
        @n
        M=M-1   // Decrement count by 1
        @SUM
        0;JMP   // Loop again
(END)
        @total
        D=M
        @R2
        M=D     // Write total to RAM[2]