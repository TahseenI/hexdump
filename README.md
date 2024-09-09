# Simple Hexdump Utility
hexdump utility for myself, just as an exercise
## Compilation instructions (C - <i>Requires GCC</i>)

### Method 1: GCC Compiler
```
gcc -o simple_hexdump hexdump.c
```

Create the /output/ folder within the same directory. That way you can use the output file functionality.

### Method 2: Makefile
```
make
...
make clean      // cleans /bin/ and /output/ folders
```

## C++ Usage
Output to command line:
```
./bin/simple_hexdump.exe <file_in>
```

Output to file:
```
./bin/simple_hexdump.exe <file_in> <file_out> 
```
Note: This utility can be used on ANY file. No extension required.

## TODOs for the future
* Replicate in other languages

* Clean up output folder and Makefile stuff
* Find a way to combine the printf and fprintf, instead of duplicating code
* Add more arguments

