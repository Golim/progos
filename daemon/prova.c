
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <string.h>
#include <unistd.h>
#include <errno.h>

#define MAXLEN (100)
 
// structure for message queue
struct mesg_buffer {
    long mesg_type;
    char mesg_text[MAXLEN];
} message;
 
int main() {
 FILE* f = fopen("./prova.txt", "a");

  fprintf(f,"ciao!\n");
  fprintf(f,"ciao!\n");
  fprintf(f,"ciao!\n");
  fprintf(f,"ciao!\n");
	fclose(f);
}