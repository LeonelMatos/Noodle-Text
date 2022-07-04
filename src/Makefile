CC = gcc # C Compiler
OUTPUT = profile_output.txt
SOURCEFILES = $(wildcard *.c)
OBJECTS = $(SOURCEFILES:%.c=%.o)

.PHONY: run, rm, clean, help

noodle : $(OBJECTS)
	@$(CC) -o $@ $^ -Wall -Wextra -pedantic -std=c99

%.o : %.c
	@$(CC) -c $< -Wall -Wextra -pedantic -std=c99

run : noodle
	@./$<

rm : 
	@rm noodle
	@echo "Removed noodle"

clean : # Clean all object .o files 
	@rm *.out
	@rm *.txt
	@echo "Cleared useless files"

a.out : main.c
	$(CC) -o -pg $^

profile : a.out # Uses gprof to profile the code 
	./$<
	gprof $< gmon.out > $(OUTPUT)

help : 
	@echo "NoodleText Makefile helper:"
	@echo "'run'     : compiles and executes the program;"
	@echo "'rm'      : removes the compiled program;"
	@echo "'clean'   : clears any auxiliary file such as .out or .txt;"
	@echo "'profile' : creates a .txt output file with results from profiling the program;"
	@echo "'help'    : I'll leave you at that one."