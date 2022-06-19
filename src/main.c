#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <unistd.h>

//Placeholder for the terminal's default settings
struct termios orig_termios;

///Resets the terminal's attributes with orig_termios
void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

///Sets the terminal's attributes
void enableRawMode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);

    struct termios raw = orig_termios;

    raw.c_lflag &= ~(ECHO);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

///The Main function
int main (void)
{
    enableRawMode();

    ///Input holder with stdin
    char in;

    int c;
    
    while (read(STDIN_FILENO, &in, 1) == 1 && in != 'q');

    while ((c = getchar()) != '\n' && c != EOF);
    disableRawMode();

    return 0;
}