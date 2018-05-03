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

static void mydaemon(){
    pid_t pid;
    pid = fork();

    if (pid < 0){
        printf("?Error while forking\n");
        exit(EXIT_FAILURE);
	}
	
    if (pid > 0){
		//Terminate parent
        exit(EXIT_SUCCESS);
	}

    if (setsid() < 0){
		printf("?Error while changing sid\n");
        exit(EXIT_FAILURE);
	}
}

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
	
	mydaemon();
	
    while (1) {
		//Daemon code		
		key_t key;
		int msgid;
		key = msgkey(0);
		msgid = msgget(key, 0666 | IPC_CREAT);
		
		msgrcv(msgid, &message, sizeof(message), 1, 0);
		msgctl(msgid, IPC_RMID, NULL);
		
		if(strstr(message.mesg_text, "quit") != NULL){
			exit(0);
		} else if(strstr(message.mesg_text, "kill") != NULL){
			exit(0);
		} else if(strstr(message.mesg_text, "delete") != NULL){
			delete_file();
			message.mesg_text[0] = '\0';
		}
		
		if(strstr(message.mesg_text, "clear") != NULL){
			//TODO: clear file
			exit(0);
		}
		
		if(strlen(message.mesg_text) > 0){
			FILE *f = fopen(dir, "a");
			if (f == NULL){
				//TODO: mandare segnale d'errore
				printf("Error opening file!\n");
				exit(1);
			}
			
			fprintf(f, "%s", message.mesg_text);
			
			fclose(f);
		}
		
		
        
        message.mesg_text[0] = '\0';
    }

    return 0;
}
