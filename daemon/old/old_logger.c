#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>


#define MAXLEN 200
 /*
// structure for message queue
struct mesg_buffer {
    long mesg_type;
    char mesg_text[MAXLEN];
	char file_name[MAXLEN];
} message;*/

struct mesg_buffer message;

int main() 
{	
	int fid = fork();

	if(fid < 0){
		printf("?ERROR while forking\n");
		exit(1);
	}
	
	if(fid == 0){
		//figlio
		key_t key = msgkey(0);
		int msgid = msgget(key, 0666);
		
		//Find if exist the logger daemon
		if(msgid == -1 && errno == ENOENT){
			printf("Starting daemon..");
			msgid = msgget(key, 0666 | IPC_CREAT); //If not, create the queue
			printf(" Started!\n");
		} else {
			exit(0);//If exist, exit
		}

		daemon(1, 1);
		
		//int end = 1;
		while(1) {
			msgrcv(msgid, &message, sizeof(message), 1, 0);
			
			write_log(&message);
			
			//clear message buffer
			message.mesg_text[0] = '\0';
		}
		
		//Elimina la message queue
		msgctl(msgid, IPC_RMID, NULL);
	}
	
	
	else {
		//padre
		long r;
		key_t key;
		int msgid;
		char msgtxt[MAXLEN];
			
		//create the queue and send messages
		key = msgkey(1);
		msgid = msgget(key, 0666);
		message.mesg_type = 1;
		
		//if(msgid == -1 && errno == ENOENT)
			//TOTO
		
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
	}
	
    return 0;
}
