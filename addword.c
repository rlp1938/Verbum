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
#include "fileops.h"

static void die(const char *inform);

int main (int argc, char** argv) {
    if (argc != 2) {
	    die("\n\tRequires one string of characters to be input\n");
    }

    char *toadd = strdup(argv[1]);
    char *wordlist;
    {
		char buf[NAME_MAX];
		sprintf(buf, "%s/.config/jumble/wordlist", getenv("HOME"));
		wordlist = strdup(buf);
	}
	fdata dat = readfile(wordlist, 0, 1);
	char *eol, *prveol, *line;
	size_t len = strlen(toadd);
	line = dat.from;
	prveol = NULL;
	/* I have to consider 3 conditions:
	 * 1. toadd < first line in the file,
	 * 2. toadd > last line in the file, and the usual,
	 * 3. toadd is be inserted someplace else. */
	while (line < dat.to) {
		eol = memchr(line, '\n', dat.to - line);
		int cmpres = strncmp(line, toadd, len);
		if (cmpres == 0) {	// error, the word exists
			die("The word to add exists.");
		} else if (cmpres > 0) {
			break;	// this is the insertion point
		}
		prveol = eol;
		line = eol + 1;
	}
	// set up my workfile
	char *tfname;
	{
		char fbuf[NAME_MAX];
		sprintf(fbuf, "%s%s%d", "addword", getenv("USER"), getpid());
		tfname = strdup(fbuf);
	}
	// format my word to add with '\n'
	char addword[NAME_MAX];
	sprintf(addword, "%s\n", toadd);
	free(toadd);
	len++;	// now longer by 1.
	// so why did I exit?
	if (line < dat.to) {
		if (prveol) {	// usual condition, inserting within the file
			writefile(tfname, dat.from, prveol+1, "w");
			writefile(tfname, addword, addword + len, "a");
			writefile(tfname, prveol+1, dat.to, "a");
		} else {	// insert at new first line in file
			writefile(tfname, addword, addword + len, "w");
			writefile(tfname, dat.from, dat.to, "a");
		}
	} else { // the insertion point was never found
		writefile(tfname, dat.from, dat.to, "w");
		writefile(tfname, addword, addword + len, "a");
	}

	if (rename(tfname, wordlist) == -1) {
		die("rename");
	}

	free(wordlist);
	free(dat.from);
	free(tfname);
    return 0;
} // main()
static void die(const char *inform)
{
	if (errno) {
		perror(inform);
	} else {
		fprintf(stderr, "%s\n", inform);
	}
	exit(EXIT_FAILURE);
} // die()
