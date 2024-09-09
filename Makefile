# Directories
DIR_BIN := bin
DIR_OUT := output

# No include files to worry about

# ---------------------------------------------
default: simple_hexdump

# Create bin/ folder
simple_hexdump_objs:
	@mkdir -p $(DIR_BIN)
	@mkdir -p $(DIR_OUT)
	gcc -c bin/ hexdump.c
	gcc -o bin/simple_hexdump hexdump.c

simple_hexdump:
	@mkdir -p $(DIR_BIN)
	@mkdir -p $(DIR_OUT)
	gcc -o bin/simple_hexdump hexdump.c

# Clean up the /bin folder
clean:
	rm -f $(DIR_BIN)/* *.o
	rmdir $(DIR_BIN)
	
	rm -f $(DIR_OUT)/*
	rmdir $(DIR_OUT)