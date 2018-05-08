#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAXLEN 200
char dir[MAXLEN];

struct mesg_buffer {
    long mesg_type;
    char mesg_text[MAXLEN];
} message;

void delete_file(){
	char cmd[MAXLEN];
	strcpy(cmd, "rm ");
	strcat(cmd, dir);
	system(cmd);
}

int main(int argc, char **argv){
   	char name[MAXLEN];
   	
	if(argc == 2){ //argv[2] = /path/to/file.txt
		strcpy(dir, argv[1]);
	} else {
		strcpy(dir, "/tmp/");
		strcpy(name, "log.txt");
		strcat(dir, name);
	}
	
	printf("Logfile: %s\n", dir);
	
	daemonize();
	
	key_t key = msgkey(0);
	int msgid = msgget(key, 0666 | IPC_CREAT);
	int continua = 0;
    while(continua == 0) {
		//Daemon code
		msgrcv(msgid, &message, sizeof(message), 1, 0);
		
		if(strstr(message.mesg_text, "quit") != NULL || strstr(message.mesg_text, "kill") != NULL)
			continua = 1;
		else if(strstr(message.mesg_text, "delete") != NULL)
		{
			delete_file();
			message.mesg_text[0] = '\0';
		} 
		else if(strstr(message.mesg_text, "clear") != NULL)
		{
			FILE *f = fopen(dir, "w");
			if (f == NULL){
				//TODO: mandare segnale d'errore
				printf("Error opening file!\n");
				continua = (1);
			}
			
			fprintf(f, "\0");
			fclose(f);
		} 
		else 
		{
			if(strlen(message.mesg_text) > 0)
			{
				FILE *f = fopen(dir, "a");
				if (f == NULL){
					//TODO: mandare segnale d'errore
					printf("Error opening file!\n");
					exit(1);
				}
				
				fprintf(f, "%s", message.mesg_text);
				
				fclose(f);
			}
		}
		
		message.mesg_text[0] = '\0';
    }
    
	// elimina la message queue
    msgctl(msgid, IPC_RMID, NULL);
	
    return 0;
}

