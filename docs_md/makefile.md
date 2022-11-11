\page makefile - Makefile

Makefile Guide
==============

# How to Run

	make

or

	make [COMMAND]

# List of Commands

Commands | Summary
:------: | :---------------------
NULL     | compiles on src/.
run      | compiles and run
rm       | removes build
clean    | removes .out/.txt files
profile  | profiling code run
build    | builds on /usr/bin
help     | show options

## Example

	make run
---

# Make Help

> NoodleText Makefile Helper
>
>	make [OPTION]
>
> Options:
>
>	'run'     : compiles and executes the program;
>
>	'rm'      : removes the compiled program;
>
>	'clean'   : clears any auxiliary file such as .out or .txt;
>
>	'profile' : creates a .txt output file with results from profiling the program;
>
>	'build'   : compile and install the program;
>
>	'help'    : I'll leave you at that one.

# Variables

Variables | Value | Description
:-------: | :---: | :----------
CC        | gcc   | C project compiler
OUTPUT    | profile_output.txt | Profiler text output
SOURCEFILES | $(wildcard *.c) | Additional code files (ex:args.c)
OBJECTS   | $(SOURCEFILES:%.c=%.o) | Compiled SOURCEFILES
