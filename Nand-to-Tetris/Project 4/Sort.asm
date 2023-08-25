// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Sort.asm

// Sorts an array that starts at the address specified in R14 with length specified in R15.
// (R14, R15 refer to RAM[14], and RAM[15] respectively.)

// Let RAM[14]+RAM[15]-1 be n and RAM[15] be counter.
// We will be using bubble sort in descending order.
//
// for (i = (num - 1); i > 0; i--)
//
//     for (j = (num - 1); j > 0; j--)
//
//         if (array[j] > array[j+1])
//
//             temp = array[j];
//             array[j] = array[j+1];
//             array[j+1] = temp;


        @R15
        D=M
        @counter
        M=D-1    // Initialize counter to the length of the array at RAM[15] minus 1
        D=M
        @RESET
        D;JNE   // goto RESET if counter is'nt 0
        @FINISH
        0;JMP

(RESET)
        @R15
        D=M
        @n
        M=D-1   // Initialize n to the length of the array at RAM[15] minus 1

(COMPARE)
        @R14
        D=M
        @n
        A=D+M     // Goes to the address of the array's end
        D=A
        @add1      // Stores the address of arr[n]
        M=D

        D=D-1       // Goes to the address of the array's end minus 1
        @add2    // Stores the address of arr[n-1]
        M=D

        @add1
        A=M
        D=M     // Get the first val
        @add2
        A=M
        D=D-M   // Calculate the value of arr[n]-arr[n-1]
        @SWAP
        D;JGT   // goto SWAP if (arr[n]-arr[n-1] > 0)
        @LOOP
        0;JMP   // goto LOOP

(SWAP)
        @add1
        A=M
        D=M
        @first_val  // Stores the value of arr[n]
        M=D

        @add2
        A=M
        D=M
        @second_val  // Stores the value of arr[n-1]
        M=D

        @second_val   // Stores the value of arr[n-1] in arr[n]
        D=M
        @add1
        A=M
        M=D

        @first_val   // Stores the value of arr[n] in arr[n-1]
        D=M
        @add2
        A=M
        M=D

(LOOP)
        @n
        M=M-1   // Decrement n by 1
        D=M
        @END
        D;JEQ   // goto END if n = 0
        @COMPARE
        0;JMP   // goto COMPARE

(END)
        @counter
        M=M-1    // Decrement counter by 1
        D=M
        @FINISH
        D;JEQ   // goto RESET if counter is'nt 0
        @RESET
        0;JMP
(FINISH)