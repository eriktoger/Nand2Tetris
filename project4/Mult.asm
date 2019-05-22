// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.
// Jag vill addera R0 till R2. och det vill jag göra R1 gånger
	
	@R2
	M=0
	@R1
	D=M
	@END
	D;JEQ
	
	(LOOP)
	@R0
	D=M
	@R2
	M=D+M
	@R1
	M=M-1 // detta är min counter--
	D=M
	@END
	D;JEQ // hade M här först men funkade inte
	@LOOP // hoopar till (LOOP)
	0;JMP
	
	(END)
		@END
		0;JMP
	

	//(LOOP)
	//if (i==n) goto END	

	//@i
	//D=M
	//@n
	//D=D-M // nu är D 0 om D var == M
	//@END // if D==0 så jump END
	//D;JEQ