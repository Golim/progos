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
#include "./logger.h"

int elaborate(msg *m)
{
	if (m->type == TYPE_EXIT)
	{
		printf("Exit message recived\n");
		return -1;
	}
	else
	{
		printf("[txt:%s , fn:%s]\n", m->msg_log.txt, m->msg_log.fn);
		
		return write_log(m);
	}
}

int write_log(msg *m)
{
	FILE *f;
	switch (m->type)
	{
	case TYPE_TXT:
		//MESSAGGIO TXT
		if (strlen(m->msg_log.fn) == 0)
			strcpy(m->msg_log.fn, DEF_TXT);
		break;

	case TYPE_CSV:
		if (strlen(m->msg_log.fn) == 0)
			strcpy(m->msg_log.fn, DEF_CSV);
		break;

	default:
		//TODO: gestisci tipo non riconosciuto!!
		if (strlen(m->msg_log.fn) == 0)
			strcpy(m->msg_log.fn, DEF_TXT);
		break;
	}

	if (strlen(m->msg_log.txt) > 0)
	{
		f = fopen(m->msg_log.fn, "a");
		if (f == NULL)
		{
			//TODO: Gestisci errore
			fprintf(stderr, "Errore nell'apertura del file!: [%s]\n", m->msg_log.fn);
			return -1;
		}
		printf("[stampato]:%s\n", m->msg_log.fn);
		fprintf(f, "%s\n", m->msg_log.txt);
		fflush(f);
		fclose(f);
	}
	return 0;
}