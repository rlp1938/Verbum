/* xword.c - if you can't win using skill; cheat!
	takes input in the form '..x..x.' where '.' represents an unknown
	character and the 'x' represent what is known
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <libgen.h>
#include <limits.h>
// Globals
#define F_OPN_FAIL	1
#define MEM_FAIL	2

static FILE *safeopen(char *path, char *mode);
static void fatalerror(int failure, char *wherewhat);

int main (int argc, char** argv) {
    FILE *fp;
    int i, ch;
    if (argc != 2) {
	    printf ("\n\tRequires one string of characters to be input\n\n");
	    return 1;
    }
    char buf[NAME_MAX];
    sprintf(buf, "%s/.config/jumble/wordlist", getenv("HOME"));
    fp = safeopen(buf, "r");

    return 0;
} // main()
FILE *safeopen(char *path, char *mode){
	FILE *sofp;
	sofp = fopen(path, mode);
	if (!(sofp)) fatalerror(F_OPN_FAIL, path);
	/*@-nullret -dependenttrans*/
	return sofp;
}// safeopen()
void  fatalerror(int failure, char *wherewhat){
	int exitcode;
	switch(failure) {
		case MEM_FAIL:
			exitcode = 3;
			(void)puts("Could not get required memory");
			break;
		case F_OPN_FAIL:
			exitcode = 2;
			(void)puts("Failed to open file");
			break;
		default:
			exitcode = -127; // should never happen
			break;
	}
	perror(wherewhat);
	exit(exitcode);
}// fatalerror()
