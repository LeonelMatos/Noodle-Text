#include <stdio.h>

/// @brief Check for additional parameters when execution the editor
/// @param arg Argument added as input from execution of the editor
/// @return -1 Invalid Input
/// @return 0 Return a confirmation of the arg
int args_check (char * arg) {

    switch (arg[1])
    {
        case 'h':
            printf("HELP\n\nNAME\n\tnoodle - NoodleText\n\n");
            printf("SYNOPSIS\n\tnoodle [FILE][-(OPTION)]\n\n");
            printf("NOTICE\n\tThis is still an early alpha version. Currently NoodleText holds very few functions and lacks content.\n\n");
            printf("DESCRIPTION\n\tNoodleText is a minimal and friendly editor. Inspired by the 'big guys' editors such as Nano and Windows\n\ttext editor, although more basic, it has the essential and the heart of a worthy text editor.\n\n");
            printf("OPTIONS\n\tFILE,\n\t\tThe editor opens the given text file.\n\n\t-h, 'help'\n\t\tA full description of the following options.\n\n");
            printf("\t-l, 'logs'\n\t\tClears the accumulated log files.\n\n");
            printf("Author\n\tLeonel Matos <https://github.com/leonelmatos/noodletext/>\n\n");
            printf("Version\n\t23.09.22   version 0.0.1\n\n\n");
            return -1;
        case 'c':
            return 1; //Verification for the editor companion for cat.c
        default:
            printf("\nInvalid Input of additional parameter\n\n");
            return -1;
    }
}