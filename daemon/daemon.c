#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAXLEN 200

//message queue structure
struct mesg_buffer {
    long mesg_type;
    char mesg_text[MAXLEN];
} message;

int main(int argc, char **argv){
   	char dir[MAXLEN];
   	char name[MAXLEN];
   	
	if(argc == 2){ //argv[2] = /path/to/file.txt
		strcpy(dir, argv[1]);
	} else {
		strcpy(dir, "/tmp/");
		strcpy(name, "log.txt");
		strcat(dir, name);
	}
	
	printf("Logfile: %s\n", dir);
	
	// Transform this process into a daemon process
	daemonize();
	
	//=============   Daemon code  ===============
	//create the queue
	key_t key = msgkey(0);
	int msgid = msgget(key, 0666 | IPC_CREAT);
	int end = 1;	

    do
	{
		// receive a message
		msgrcv(msgid, &message, sizeof(message), 1, 0);
		
		// parse message
		if(strstr(message.mesg_text, "quit") != NULL)
			end = 1;
		else
		{
			if(strlen(message.mesg_text) > 0)
			{
				// write message on file
				FILE *f = fopen(dir, "a");
				if (f == NULL){
					//TODO: mandare segnale d'errore
					perror("Error opening file!\n");
					exit(1);
				}
				
				fprintf(f, "%s", message.mesg_text);
				fclose(f);
			}
			
			message.mesg_text[0] = '\0';
		}
		
    }while(end);

	//destroy the message queue
	msgctl(msgid, IPC_RMID, NULL);
	//============================================

    return 0;
}
