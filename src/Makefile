CC = gcc # C Compiler
OUTPUT = profile_output.txt

.PHONY: run, rm, clean

noodle.exe : main.c
	@$(CC) -o $@ $^ -Wall -Wextra -pedantic -std=c99

run : noodle.exe
	@./$<

rm : 
	@rm noodle.exe
	@echo "Removed noodle.exe"

clean : # Clean all object .o files 
	@rm *.out
	@rm *.txt
	@echo "Cleared useless files"

a.out : main.c
	$(CC) -o -pg $^

profile : a.out # Uses gprof to profile the code 
	./$<
	gprof $< gmon.out > $(OUTPUT)