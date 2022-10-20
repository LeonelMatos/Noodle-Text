#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main () {

	char cat[] = "  Noodle\n   /\\_/\\ \n  (º_._º)\n  /|´ `|\\ \n    | |\n";

	printf("%s", cat);

	return 0;
}

/* TEST
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
