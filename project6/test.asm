// Program: fill.asm
// Fills CPU emulator screen when any key on keyboard is pressed
// Usage: Click and hold any key on keyboard to fill the screen
// Make sure to toggle 'Enable Keyboard' in CPU emulator to listner for inputs


(OBSERVE)
	@SCREEN // reset i to 16584
	D=A
	@i
	M=D

	@KBD // key <= 0
    @2 //test comment here
	D=M
	@OFF
	D;JLE
	@FILL
	D;JGT

(OFF)
	@i // 24576 - i <= 0
	D=M
	@KBD
	D=A-D
	@OBSERVE
	D;JLE

	@i
	A=M
	M=0 // 0 dec = 0x0000 hex = 0000000000000000 bit

	@i // i++
	M=M+1
    @6
	@OFF
	0;JMP

(FILL) 	
	@i // 24576 - i <= 0
	D=M
	@KBD
	D=A-D
	@OBSERVE
	D;JLE

	@i
	A=M
	M=-1 // -1 dec = 0xFFFF hex = 1111111111111111 bit

	@i // i++
	M=M+1
	@FILL
	0;JMP
