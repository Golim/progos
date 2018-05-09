#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
<<<<<<< HEAD:daemon/old/writer.c
#define MAXLEN 200
=======

#include <errno.h>

#define MAXLEN (100)
>>>>>>> ponza:daemon/writer.c
 
// structure for message queue
struct mesg_buffer {
    long mesg_type;
    char mesg_text[MAXLEN];
	char file_name[MAXLEN];
} message;
<<<<<<< HEAD:daemon/old/writer.c
 
=======

>>>>>>> ponza:daemon/writer.c
int main() {
    long r;
    key_t key;
	int msgid;
	char msgtxt[MAXLEN];
    
<<<<<<< HEAD:daemon/old/writer.c
	//create the queue and send messages
	key = msgkey(1);
	msgid = msgget(key, 0666);
	message.mesg_type = 1;
	
	if(msgid == -1 && errno == ENOENT) {
		printf("Daemon is not running\nDaemon starting...");
=======
    //create the queue and send messages
	key = msgkey(1);
	msgid = msgget(key, 0666);
>>>>>>> ponza:daemon/writer.c

	if(msgid == -1 && errno == ENOENT)
    {
		printf("Daemon is not running\nDaemon starting...");
		char *args[] = {NULL, NULL};
		char *cmd = malloc(MAXLEN);
		getcwd(cmd, MAXLEN);
		strcpy(cmd, "/mydaemon");
		system(cmd);
		printf(" Started!\n");
	}
<<<<<<< HEAD:daemon/old/writer.c
	
=======

	message.mesg_type = 1;
    
>>>>>>> ponza:daemon/writer.c
    while(1){
		
		printf("> ");
		r=(long)fgets(msgtxt, MAXLEN, stdin);
		
		if(strstr(msgtxt, "exit") != NULL){
			exit(0);
		}
		
		strcpy(message.mesg_text, msgtxt);
		msgsnd(msgid, &message, sizeof(message), 0);
		
		if(strstr(msgtxt, "quit") != NULL){
			exit(0);
		}
	}
<<<<<<< HEAD:daemon/old/writer.c
	
    return 0;
}
=======
    return 0;
}
>>>>>>> ponza:daemon/writer.c
