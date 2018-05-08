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
const char *DEF_TXT = "log.txt";
const char *DEF_CSV = "log.csv";

struct mesg_buffer {
    long mesg_type;
    char mesg_text[MAXLEN];
	char file_name[MAXLEN];
} message;

int main(int argc, char **argv){
   	char name[MAXLEN];
	
	daemon(1, 1);
	
	key_t key = msgkey(0);
	int msgid = msgget(key, 0666 | IPC_CREAT);
	
	int end = 1;
    while(end) {
		msgrcv(msgid, &message, sizeof(message), 1, 0);
		
		if(strlen(message.mesg_text) > 0) {
			if(strstr(message.mesg_text, "quit") != NULL || strstr(message.mesg_text, "kill") != NULL) {
				end = 0;
			} else {
				//if message type = 1 --> txt
				//if message type = 2 --> csv
				//if name file is empty keep the default log file
				
				// Dave997
				FILE *f;
				if(strlen(message.file_name) == 0){
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
					end = 0;
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
	
    return 0;
}

