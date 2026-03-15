# Hack assembler

Program in C to parse hack assembly language into hack CPU binary instructions.

## Building
To build a binary just run:
```bash
./build.sh
```

## Usage

```bash
./hack_assembler < input/Pong.asm > output/pong.hack
```
You can import outputted **pong.hack** file into nand2tetris online IDE.

## Limitations

Currently assember does not support multi-line comments in assembly files. Use **//** aka line comments.
Error/input handling.
May have some not encountered bugs (first time in C, please don't judge ;)).

## References

https://drive.google.com/file/d/1CITliwTJzq19ibBF5EeuNBZ3MJ01dKoI/view
https://b1391bd6-da3d-477d-8c01-38cdf774495a.filesusr.com/ugd/44046b_89a8e226476741a3b7c5204575b8a0b2.pdf
