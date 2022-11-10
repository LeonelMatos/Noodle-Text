#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main () {

	char cat[] = "  Noodle\n   /\\_/\\ \n  (º_._º)\n  /|´ `|\\ \n    | |\n";

	printf("%s", cat);

	return 0;
}

/// @brief (Debug) Draw function for the on-built editor companion
/// @param ab dynamic string buffer for writing
/// @param numrows number of rows in the editor window
/// @param screencols number of collumns in the editor window
void drawCat (struct abuf *ab, const unsigned int numrows, const unsigned int screencols) {
    char cat[50];
                int cat_len = snprintf(cat, sizeof(cat), 
                "\r /\\_/\\\n\r(º . º)/\n\r/| ^ |\n\r  | |", numrows);

                if (cat_len > screencols) cat_len = screencols;

                int cat_pad = (screencols - cat_len) / 1;

                if (cat_pad) {abAppend(ab, "*", 1); cat_pad--;}
                while (cat_pad--)
                    abAppend(ab, " ", 1);
                
                abAppend(ab, cat, cat_len);

                //End of cat
}

/* TEST - Debug 
// INSERT CAT HERE
                
                char cat[50];
                int cat_len = snprintf(cat, sizeof(cat), 
                "\r /\\_/\\\n\r(º . º)/\n\r/| ^ |\n\r  | |", E.numrows);

                if (cat_len > E.screencols) cat_len = E.screencols;

                int cat_pad = (E.screencols - cat_len) / 1;

                if (cat_pad) {abAppend(ab, "*", 1); cat_pad--;}
                while (cat_pad--)
                    abAppend(ab, " ", 1);
                
                abAppend(ab, cat, cat_len);

                //End of cat

*/

//   /\ /\
//  (º w º)/
//  /|   |
//    | |

//   /\_/\
//  (º w º)__
//  /|   |\
//    | |
