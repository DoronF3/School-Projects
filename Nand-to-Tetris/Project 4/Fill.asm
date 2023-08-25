// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

        @KBD
        D=A
        @0
        M=D
        @SCREEN
        D=A
        @0
        M=M-D   // Calculate KBD - SCREEN = 8192 and put in RAM[0]

(LOOP)
        @0
        D=M
        @i
        M=D    // initialize i=8192

(PRESSED)
        @KBD
        D=M
        @WHITEN
        D;JEQ   // Check if no key is being pressed (value 0), goto WHITEN

(BLACKEN)
        @i
        D=M
        @KBD
        A=A-D   // Calculate addresses
        M=-1    // Turn to black
        @END
        0;JMP // goto END

(WHITEN)
        @i
        D=M
        @KBD
        A=A-D   // Calculate addresses
        M=0    // Turn to white

(END)
        @i
        MD=M-1  // Decrement i by 1
        @LOOP
        D;JEQ   // goto LOOP if finished iterating
        @PRESSED
        0;JMP   // goto PRESSED

