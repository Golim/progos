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

<<<<<<< HEAD
static void daemonize(){
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

=======
>>>>>>> 47ddef8227ecea98803b976dd5445f904f9bfa7c
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
	
<<<<<<< HEAD
	daemonize();
	
	key_t key = msgkey(0);
	int msgid = msgget(key, 0666 | IPC_CREAT);
	
    while(1) {
		//Daemon code
		msgrcv(msgid, &message, sizeof(message), 1, 0);
		
		if(strstr(message.mesg_text, "quit") != NULL){
			exit(1);
		} else if(strstr(message.mesg_text, "kill") != NULL){
			exit(1);
		} else if(strstr(message.mesg_text, "delete") != NULL){
			delete_file();
			message.mesg_text[0] = '\0';
		} else if(strstr(message.mesg_text, "clear") != NULL){
			//TODO: clear file
			exit(0);
		} else {
			if(strlen(message.mesg_text) > 0){
				FILE *f = fopen(dir, "a");
				if (f == NULL){
					//TODO: mandare segnale d'errore
					printf("Error opening file!\n");
=======
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
>>>>>>> 47ddef8227ecea98803b976dd5445f904f9bfa7c
					exit(1);
				}
				
				fprintf(f, "%s", message.mesg_text);
<<<<<<< HEAD
				
				fclose(f);
			}
		}
		
		message.mesg_text[0] = '\0';
    }
    
    msgctl(msgid, IPC_RMID, NULL);
	
=======
				fclose(f);
			}
			
			message.mesg_text[0] = '\0';
		}
		
    }while(end);

	//destroy the message queue
	msgctl(msgid, IPC_RMID, NULL);
	//============================================

>>>>>>> 47ddef8227ecea98803b976dd5445f904f9bfa7c
    return 0;
}
