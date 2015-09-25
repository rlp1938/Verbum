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
static int compare(const char *buf, const char *totest);
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

    ch = fgetc(fp);
    i = 0;
    while(!feof(fp)) {
	    if (ch == 10){
		    buf[i] = 0;
		    i = 0;
		    if (compare(buf, argv[1]))
			    printf("%s\n", buf);
	    } else { // add the char to the buffer
		    buf[i++] = ch;
	    }
	ch = fgetc(fp);
    }// while(...compare(buf, argv[1])
    if (i>0) { // the last word in the dictionary
	    buf[i] = 0;
	    if (compare(buf, argv[1]))
		    printf("%s\n", buf);
    }
    return 0;
} // main()
int compare(const char *buf, const char *totest) {
	int ch;
	size_t l, i;
	l = strlen(totest);
	if (l != strlen(buf))
		return 0;
	for (i=0; i<l; i++) {
		ch = tolower(totest[i]);
		if (ch == '.')
			continue;
		if (ch != buf[i])
			return 0;
	}
	return 1;
}// compare(...
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
