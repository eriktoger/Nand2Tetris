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

// Put your code here.

 	

	(LOOP) 
		@KBD
		D=M
		@R3
		M=-1
		@BLACK
		D;JGT
		@WHITE
		D;JEQ

		
	
	(WHITE)
		@R3 // h�r sparas vilken iteration jag �r p�
		M=M+1 // �kar med en
		@SCREEN // hittar platsen f�r SCREEN
		D=A // Sparar den
		@R3
		D=D+M // vilken rad jag �r p�
		A=D // detta �r den 16-bit som ska s�ttas till noll/ett f�r vit/svart
		M=0 // vit (-1 �r svart)
		@24575 //  sista raden
		D=D-A // Om vi �r p� sista raden s� ska vi avsluta
		@WHITE // loopar tills jag �r klar
		D;JNE
		@LOOP // sedan hoppar vi tillbaka
		0;JMP
	(BLACK)
		
		@R3
		M=M+1
		@SCREEN
		D=A
		@R3
		D=D+M
		A=D 
		M=-1
		@24575 
		D=D-A 
		@BLACK
		D;JNE
		@LOOP
		0;JMP
