/*---INCLUDE---*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>

/*---DEFINES---*/

/// Value that represents the CTRL key
#define CTRL_KEY(k) ((k)&0x1f)

/*---DATA---*/

struct editorConfig
{
    // Placeholder for the terminal's default settings
    struct termios orig_termios;

};

struct editorConfig E;

///=die. Error handling function using unistd.h \arg \c (string) s: error id name
void kill(const char *s)
{
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);

    perror(s);
    exit(1);
}

/*---TERMINAL OPTIONS---*/
/// Resets the terminal's attributes with orig_termios
void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1)
        kill("tcsetattr");
}

/// Sets the terminal's attributes
void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &E.orig_termios) == -1)
        kill("tcgetattr");
    atexit(disableRawMode);

    struct termios raw = E.orig_termios;

    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        kill("tcgetattr");
}

/// Handles the reading of STDIN and stores a char at a time in input.
/// \return \c(char) input: key input from STDIN
char editorReadKey()
{
    int nread;
    char input;
    while ((nread = read(STDIN_FILENO, &input, 1)) != 1)
    {
        if (nread == -1 && errno != EAGAIN)
            kill("read");
    }
    return input;
}

///Gives the size of the terminal \arg (*int) rows: window rows
///\arg (*int) cols: window collumns
int getWindowSize(int *rows, int *cols)
{
    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
        return -1;
    else {
        *cols = ws.ws_col;
        *rows = ws.ws_row;
        return 0;
    }
}


/*---OUTPUT---*/

/// Draws the rows on the side of the editor
void editorDrawRows()
{
    /// Number of rows to draw
    int y;
    for (y = 0; y < 24; y++)
        write(STDOUT_FILENO, ".\r\n", 3);
}

/// Clears the screen of the editor
void editorRefreshScreen()
{
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);

    editorDrawRows();

    write(STDOUT_FILENO, "\x1b[H", 3);
}

/*---INPUT---*/

/// Processes the result/command of each key
/// Calls the \c editorReadKey() function to receive
/// the user's input
void editorProcessKeypress()
{
    /// Input holder with stdin
    char input = editorReadKey();

    switch (input)
    {
    case CTRL_KEY('q'):
        write(STDOUT_FILENO, "\x1b[2J", 4);
        write(STDOUT_FILENO, "\x1b[H", 3);
        exit(0);
        break;
    }
}

/*---INIT---*/

/// The Main function
int main(void)
{
    enableRawMode();

    while (1)
    {
        editorRefreshScreen();
        editorProcessKeypress();
    }

    printf("\n");
    return 0;
}