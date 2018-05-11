#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/wait.h>

#include "../util.h"
#include "../messaging/messaging.h"
#include "./logger.h"

/**
 * 
 * Padre: analizzatore comandi
 * Figlio: serve solo a generare il demone
 * Nipote: logger (demone)
 * 
 * */

FILE* logger_logs;
time_t t;
struct tm tm;

int main()
{
	logger_logs = fopen("/tmp/logger_logs.txt", "a");
	t = time(NULL);
	tm = *localtime(&t);

	fprintf(logger_logs, "%d:%d:%d) logger started\n", tm.tm_hour, tm.tm_min, tm.tm_sec);

	int fid = fork(); //Genero figlio
	msg message;
	if (fid < 0)
	{
		printf("Errore nel fork iniziale \n");
		exit(1);
	}

	if (fid == 0)
	{
		fprintf(logger_logs, "%d:%d:%d) figlio generato\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
		//Figlio
		int fid2 = fork();
		
		if(fid < 0)
		{
			fprintf(logger_logs, "%d:%d:%d) Daemon fork FAILED!\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
			exit(1);
		}

		if(fid == 0)	//Nipote (demone)
		{
				fprintf(logger_logs, "%d:%d:%d) nipote generato\n", tm.tm_hour, tm.tm_min, tm.tm_sec);

				//msg message;
				daemon(1, 1); //unistd.h

				//Creo la message queue
				key_t key = msgkey(0);
				int msgid = msgget(key, 0666 | IPC_CREAT);

				while (strcmp(message.mesg_text, "quit") != 0)
				{
					msgrcv(msgid, &message, sizeof(message), 1, 0);
					write_log(&message);
					//clear message buffer
					message.mesg_text[0] = '\0';
				}

				//Elimina la message queue
				msgctl(msgid, IPC_RMID, NULL);
		}
		else //Figlio
		{ 
			fprintf(logger_logs, "%d:%d:%d) figlio muore\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
			exit(0); //L'unica funzione del figlio e' creare il nipote e morire
		}//end if
	}
	else
	{
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

		while (msgid == -1 && errno == ENOENT)
		{
			printf("La coda non esiste ancora, attendo un po'...\n");
			sleep(2);
		}

		while (1)
		{
			printf("> ");
			r = (long)fgets(msgtxt, MAXLEN, stdin);

			// terminate logger
			if (strstr(msgtxt, "exit") != NULL)
			{
				fprintf(logger_logs, "%d:%d:%d) logger terminated\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
				exit(0);
			}

			strcpy(message.mesg_text, msgtxt);
			msgsnd(msgid, &message, sizeof(message), 0);

			// terminate logger and kill daemon
			if (strstr(msgtxt, "quit") != NULL)
			{
				fprintf(logger_logs, "%d:%d:%d) logger terminated and daemon killed\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
				exit(0);
			}
		}
	}

	return 0;
}


void write_log(msg *m)
{
	FILE* f;

	fprintf(logger_logs, "%d:%d:%d) message received \"%s\"\n", tm.tm_hour, tm.tm_min, tm.tm_sec, m->mesg_text);
	
	if(strlen(m->mesg_text) > 0) 
	{
		if(strstr(m->mesg_text, "quit") != NULL || strstr(m->mesg_text, "kill") != NULL) 
		{
			fprintf(logger_logs, "%d:%d:%d) logger terminated\n", tm.tm_hour, tm.tm_min, tm.tm_sec, m->mesg_text);
			exit(0);
		} 
		else 
		{
			if(!strlen(m->file_name) > 0)
			{
				/*
					if message type = 1 --> txt
					if message type = 2 --> csv
					if name file is empty keep the default log file
				*/
				switch(m->mesg_type)
				{
					case 1:
						f = fopen(DEF_TXT, "a");
						fprintf(logger_logs, "%d:%d:%d) message recorded in %s\n", tm.tm_hour, tm.tm_min, tm.tm_sec, m->mesg_text, m->file_name);
						fprintf(f, "%s\n", m->mesg_text);
						break;
					case 2:
						f = fopen(DEF_CSV, "a");
						fprintf(logger_logs, "%d:%d:%d) message recorded in %s\n", tm.tm_hour, tm.tm_min, tm.tm_sec, m->mesg_text, m->file_name);
						fprintf(f, "%s", m->mesg_text);
						break;
					default:
						fprintf(logger_logs, "%d:%d:%d) ERR: mesg_type not recognized!\n", tm.tm_hour, tm.tm_min, tm.tm_sec, m->mesg_text);
						break;
				}//end switch
			}
			else
			{
				switch(m->mesg_type)
				{
					case 1:
						f = fopen(m->file_name, "a");
						fprintf(logger_logs, "%d:%d:%d) message recorded in %s\n", tm.tm_hour, tm.tm_min, tm.tm_sec, m->mesg_text, m->file_name);
						break;
					case 2:
						f = fopen(m->file_name, "a");
						fprintf(logger_logs, "%d:%d:%d) message recorded in %s\n", tm.tm_hour, tm.tm_min, tm.tm_sec, m->mesg_text, m->file_name);
						break;
					default:
						fprintf(logger_logs, "%d:%d:%d) ERR: mesg_type not recognized!\n", tm.tm_hour, tm.tm_min, tm.tm_sec, m->mesg_text);
						break;
				}//end switch
			}//end if(!strlen(msg->file_name) > 0)
		}//end if(strstr...!=NULL)
	}//end if(strlen(message.mesg_text) > 0) 

}//end write_log