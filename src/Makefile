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
	@rm *.o
	@echo "Removed noodle"

clean : # Clean all object .o files 
	@rm *.out
	@rm *.txt
	@echo "Cleared useless files"

a.out : $(OBJECTS)
	$(CC) -pg $^ 

profile : a.out # Uses gprof to profile the code 
	./$<
	gprof $< gmon.out > $(OUTPUT)

build : noodle
	@sudo mv noodle /usr/bin

help : 
	@echo "\n\033[1m"NoodleText Makefile Helper"\033[0m\n"
	@echo "\tmake [OPTION]"
	@echo "\n\033[1m"Options:"\033[0m\n"
	@echo "\t'run'     : compiles and executes the program;"
	@echo "\t'rm'      : removes the compiled program;"
	@echo "\t'clean'   : clears any auxiliary file such as .out or .txt;"
	@echo "\t'profile' : creates a .txt output file with results from profiling the program;"
	@echo "\t'build'   : compile and install the program;"
	@echo "\t'help'    : I'll leave you at that one.\n\n"
