// Program: multiplication.asm
// Multiplies two numbers stored inside RAM[0] and RAM[1] addresses 
// and puts result inside RAM[2]: RAM[2] = RAM[0] * RAM[1]
// Usage: Put numbers inside RAM[0] and RAM[1] which you want to multiply

// Possible performance upgrades: 
// 1. Check for 0's and end program instantly
// 2. Before looping pick smallest number


@R2 //RAM[2] = 0
M=0

@i
M=0

@R1
D=M
@n
M=D

@INVERT // if RAM[1] < 0
D;JLT

(LOOP)
	@i // loop condition
	D=M
	@n
	D=D-M
	@END
	D;JGE

	@R1
	D=M
	@NEGATIVE
	D;JLT

	(POSITIVE)
		@R0
		D=M
		@R2
		M=D+M
		@ITERATOR
		0;JMP

	(NEGATIVE)
		@R0
		D=M
		@R2
		M=M-D
		@ITERATOR
		0;JMP

	
	(ITERATOR)
		@i // i++
		M=M+1
		@LOOP
		0;JMP

(INVERT) // n = !RAM[1]
	@n
	D=!M
	M=D+1
	@LOOP
	0;JMP

(END)
	@END
	0;JMP