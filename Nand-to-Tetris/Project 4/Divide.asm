//The program input will be at R13,R14 while the result R13/R14 will be stored at R15. Don't change the input registers.
//The remainder should be discarded.
//You may assume both numbers are positive and larger than 0.

// Let R13 = n mone, R14=m mechane 
// c=1; a=0;              n/m
// while c*m <=n do
// 		c=2*c
// while c!=1 do
// 		c=c/2
//		if (a+c)*m>n then
//			skip
//		else
//			a=a+c
// return a <==> R15=a

        @R15
        M=0     // Initialize RAM[15] to be 0
        @R13
        D=M
        @n
        M=D     // Initialize n to the value at RAM[13]
        @R14
        D=M
        @m
        M=D     // Initialize n to the value at RAM[14]
        @m_tmp
        M=D     // Initialize m_temp to the value of m
        @counter
        M=1     // Initialize counter to be 1

(CONDITION1)

        @m_tmp
        M=M<<
        D=M     // Calculate m*2
        @n
        D=D-M
        @LOOP1
        D;JLE   // while  (2m-n<=0)
        @CONDITION2
        0;JMP   // goto CONDITION2
	
(LOOP1)

        @m
        M=M<<   // Shift left m
        D=M
        @m_tmp
        M=D     // Copy the value to m_temp
        @counter
        M=M<<   // Shift left counter
        @CONDITION1
        0;JMP   // goto CONDITION1
	
(CONDITION2)

        @counter
        D=M
        @CONDITION3
        D;JGT   // if counter > 0 goto CONDITION3
        @END
        0;JMP   // lse goto END

(CONDITION3)

        @counter
        D=M
        M=M>>
        @counter_tmp
        M=D
        @m
        D=M
        M=M>>
        @m_tmp
        M=D
        @n
        D=M
        @m_tmp
        D=D-M
        @IF_STATEMENT
        D;JGE   // if (n-m >= 0) goto IF_STATEMENT
        @CONDITION2
        0;JMP   // else goto CONDITION2
	
(IF_STATEMENT)

        @m
        D=M
        D=D<<
        @n
        M=M-D   // n-=m
        @counter_tmp
        D=M
        @R15
        M=M+D   // result += c
        @CONDITION2
        0;JMP   // goto CONDITION2
	

(END)

        0;JMP
	