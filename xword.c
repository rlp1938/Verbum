/* xword.c - if you can't win using skill; cheat!
	takes input in the form '..x..x.' where '.' represents an unknown
	character and the 'x' represent what is known
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int compare(const char *buf, const char *totest);

int main (int argc, char** argv) {
    FILE *fp;
    char buf[40];
    int i, ch, li;
    if (argc != 2) {
	    printf ("\n\tRequires one string of characters to be input\n\n");
	    return 1;
    }
    fp = fopen("/usr/local/etc/mydict","r");
    li=strlen(argv[1]);
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
	int i,l,ch;
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
