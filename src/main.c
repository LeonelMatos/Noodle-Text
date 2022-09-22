/*---INCLUDE---*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>

#include "args.h"

/*---DEFINES---*/

/// Value that represents the CTRL key
#define CTRL_KEY(k) ((k)&0x1f)

#define NOODLE_VERSION "0.0.1"

/*---DATA---*/

struct editorConfig
{
    int screenrows;
    int screencols;
    // Placeholder for the terminal's default settings
    struct termios orig_termios;

};

struct editorConfig E;

///=die. Error handling function using unistd.h \arg (string) s: error id name
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

int getCursorPosition (int *rows, int *cols) {
    char buf[32];
    unsigned int i = 0;

    if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) return -1;

    while (i < sizeof(buf) - 1) {
        if (read(STDIN_FILENO, &buf[i], 1) != 1) break;
        if (buf[i] == 'R') break;
        i++;
    }
    buf[i] = '\0';

    if (buf[0] != '\x1b' || buf[1] != '[') return -1;
    if (sscanf(&buf[2], "%d;%d", rows, cols) != 2) return -1;

    return 0;
}

///Gives the size of the terminal \arg (*int) rows: window rows
///\arg (*int) cols: window collumns
int getWindowSize(int *rows, int *cols)
{
    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
    {
        //Moves the cursor forward/down to the down-right corner of the terminal
        if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) return -1;
        return getCursorPosition(rows, cols);
    }
    else {
        *cols = ws.ws_col;
        *rows = ws.ws_row;
        return 0;
    }
}

/*---APPEND_BUFFER---*/

//Definitions
/// Append buffer: dynamic string type struct.
struct abuf {
    /// Char pointer to the buffer in memory with length 
    char *b;
    /// int length value of the buffer
    int len;
};

/// Represents an empty buffer, used as a constructor
///for the abuf type struct
#define ABUF_INIT {NULL, 0}

//Functions

/// Appends a string \arg s to the a given \arg abuf struct 
///with a \arg len size by allocating enough memory for \c s
void abAppend (struct abuf *ab, const char *s, int len)
{
    char *new = realloc (ab->b, ab->len + len);

    if (new == NULL) return;

    memcpy(&new[ab->len], s, len);
    ab->b = new;
    ab->len += len;

}

/// Deallocates the dynamic memory used by the given \arg ab abuf struct
void abFree (struct abuf *ab)
{
    free(ab->b);

}


/*---OUTPUT---*/

/// Draws the rows on the side of the editor
void editorDrawRows(struct abuf *ab)
{
    /// Number of rows to draw
    int y;
    for (y = 0; y < E.screenrows; y++) {
        if (y == E.screenrows / 3)
        {
            char welcome[80];
            int welcome_len = snprintf(welcome, sizeof(welcome), "NoodleText || version %s", NOODLE_VERSION);

            if (welcome_len > E.screencols) welcome_len = E.screencols;

            int padding = (E.screencols - welcome_len) / 2;

            if (padding) {
                abAppend(ab, "*", 1);
                padding--;
            }
            while (padding--){ abAppend(ab, " ", 1);}

            abAppend(ab, welcome, welcome_len);
        }
        else {
            abAppend(ab, "*", 1);
        }

        abAppend(ab, "*", 1);

        abAppend(ab, "\x1b[K", 3);

        if (y < E.screenrows - 1)
            abAppend(ab, "\r\n", 2);

    }
}

/// Clears the screen of the editor
void editorRefreshScreen()
{
    struct abuf ab = ABUF_INIT; // Creates the abuf buffer with INIT
    // Instead of always using write(STDOUT_FILENO,...), the abAppend(...) will
    // gather the text and write it all at once

    abAppend(&ab, "\x1b[?25l", 6); // Adds the string to the buffer
    abAppend(&ab, "\x1b[H", 3);

    editorDrawRows(&ab);

    abAppend(&ab, "\x1b[H", 3);
    abAppend(&ab, "\x1b[?25h", 6);

    write(STDOUT_FILENO, ab.b, ab.len); // Writes out all the accumulated strings

    abFree(&ab); // After writing, frees the allocated memory
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

/// Initialize all the fields in the E struct after enabling raw mode in the editor
void initEditor() {
    if (getWindowSize(&E.screenrows, &E.screencols) == -1) kill("getWindowSize");
}

/// The Main function.
///\arg Receives arguments when called, used as options
int main(int argc, char *argv[])
{
    //Check arguments for extra options
    if (argc != 1) {
        if (args_check (argv[1]) == -1) return 0;
    }

    enableRawMode();
    initEditor();

    while (1)
    {
        editorRefreshScreen();
        editorProcessKeypress();
    }

    printf("\n");
    return 0;
}