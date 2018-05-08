#include "config.h"

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

int check_daemon_exist();
 
int main() {
    long r;
    key_t key;
	int msgid;
	char msgtxt[MAXLEN];

	key = msgkey(1);
	msgid = msgget(key, 0666);

  printf("[Errore] %d - %d \n ", msgid, errno);  
	  
}

int check_daemon_exist(){
	int size = 0;
	
	system("ps -xj | grep -v grep | grep mydaemon >> /tmp/.check");
	FILE *fp = fopen("/tmp/.check", "r");
	if (fp != NULL) {
		fseek (fp, 0, SEEK_END);
		size = ftell(fp);
	}
	fclose(fp);
	system("rm /tmp/.check");
	
	return size;
}
