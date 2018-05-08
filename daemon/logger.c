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
 
// structure for message queue
struct mesg_buffer {
    long mesg_type;
    char mesg_text[MAXLEN];
	char file_name[MAXLEN];
} message;

const char *DEF_TXT = "log.txt";
const char *DEF_CSV = "log.csv";

int main() {
	
	//primo fork
	int fid = fork();
	if(fid < 0){
		printf("?ERROR\n");
		exit(1);
	}
	
	if(fid == 0){
		//figlio
		daemon(1, 1);
		key_t key = msgkey(0);
		int msgid = msgget(key, 0666 | IPC_CREAT);
		
		//Find if exist the logger daemon
		//if(msgid == -1 && errno == ENOENT)
			//msgid = msgget(key, 0666 | IPC_CREAT); //If not, create the queue
		
		//int end = 1;
		while(1) {
			msgrcv(msgid, &message, sizeof(message), 1, 0);
			
			if(strlen(message.mesg_text) > 0) {
				if(strstr(message.mesg_text, "quit") != NULL || strstr(message.mesg_text, "kill") != NULL) {
					//end = 0;
					msgctl(msgid, IPC_RMID, NULL);
					exit(0);
				} else {
					//if message type = 1 --> txt
					//if message type = 2 --> csv
					//if name file is empty keep the default log file
					FILE *f;
					if(!strlen(message.file_name) > 0){
						switch(message.mesg_type){
							case 1:
								f = fopen(DEF_TXT, "a");
								break;
							case 2:
								f = fopen(DEF_CSV, "a");
								break;
							default:
								//Non dovrebbe succedere
								f = fopen(DEF_TXT, "a");
								break;
						}
					} else {
						f = fopen(message.file_name, "a");
					}
					

					
					if (f == NULL){
						//TODO: mandare segnale d'errore
						printf("Error opening file!\n");
						//end = 0;
						msgctl(msgid, IPC_RMID, NULL);
						exit(0);
					} else {
						fprintf(f, "%s", message.mesg_text);
					}

					fclose(f);
				}
			}
			//Di dubbia utilita
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
