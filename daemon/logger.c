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
#include <sys/wait.h>

#define MAXLEN 200

struct mesg_buffer message;

/**
 * 
 * Padre: analizzatore comandi
 * Figlio: serve solo a generare il demone
 * Nipote: logger (demone)
 * 
 * */

int main() {
	int fid = fork(); //Genero figlio
	
	if(fid < 0){
		printf("?ERROR\n");
		exit(1);
	}
	
	if(fid == 0){
		//Figlio
		int fid2 = fork(); //Genero nipote
		
		if(fid2 < 0){
			printf("?ERROR 2\n");
			exit(1);
		}
		
		if (fid2 == 0){
			//Nipote (demone)
			
			daemon(1, 1);
			
			key_t key = msgkey(0);
			int msgid = msgget(key, 0666 | IPC_CREAT);
			
			/*
			if(msgid == -1 && errno == ENOENT){
				msgid = msgget(key, 0666 | IPC_CREAT);
			} else {
				exit(0);
			}	
			*/
			while(1) {
				msgrcv(msgid, &message, sizeof(message), 1, 0);
				
				write_log(&message);
				
				//clear message buffer
				message.mesg_text[0] = '\0';
			}
			
			//Elimina la message queue
			msgctl(msgid, IPC_RMID, NULL);
		} else {
			//Figlio
			exit(0); //L'unica funzione del figlio e' creare il nipote
		}
	} else {
		//padre
		wait(NULL); //Aspetto che venga creato il demone
		
		long r;
		key_t key;
		int msgid;
		char msgtxt[MAXLEN];
			
		//create the queue and send messages
		key = msgkey(1);
		msgid = msgget(key, 0666);
		message.mesg_type = 1;
		
		while(msgid == -1 && errno == ENOENT){
			printf("La coda non esiste ancora, attendo un po'...\n");
			sleep(2);
		}
		
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
