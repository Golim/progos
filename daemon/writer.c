#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <string.h>
#include <unistd.h>
#define MAXLEN (100)
 
// structure for message queue
struct mesg_buffer {
    long mesg_type;
    char mesg_text[MAXLEN];
} message;

int check_daemon_exist();
 
int main() 
{
    long r;
<<<<<<< HEAD
    key_t key;
=======
	key_t key;
>>>>>>> 47ddef8227ecea98803b976dd5445f904f9bfa7c
	int msgid;
	char msgtxt[MAXLEN];
    
    if(check_daemon_exist() == 0){
		printf("Daemon is not running\n");
		exit(1);
	}
	
	//create the queue and send messages
	key = msgkey(1);
	msgid = msgget(key, 0666 | IPC_CREAT);
	message.mesg_type = 1;
    
<<<<<<< HEAD
    while(1){
		
		printf("> ");
=======
	//create the queue and send messages
	key = msgkey(1);
	msgid = msgget(key, 0666 | IPC_CREAT);
	message.mesg_type = 1;
    while(1)
	{
		//read string
		printf("Input data: ");
>>>>>>> 47ddef8227ecea98803b976dd5445f904f9bfa7c
		r=(long)fgets(msgtxt, MAXLEN, stdin);
		strcpy(message.mesg_text, msgtxt);

		//send string
		msgsnd(msgid, &message, sizeof(message), 0);
		printf("\n");
		
		if(strstr(msgtxt, "quit") != NULL){
			exit(0);
		}
	}
    return 0;
}

int check_daemon_exist(){
	int size = 0;
	
	system("ps -xj | grep -v grep | grep firstdaemon >> /tmp/.check");
	FILE *fp = fopen("/tmp/.check", "r");
	if (fp != NULL) {
		fseek (fp, 0, SEEK_END);
		size = ftell(fp);
	}
	fclose(fp);
	system("rm /tmp/.check");
	
	return size;
}
