#include <stdio.h>
#include <string.h>

#include "./logger.h"

int write_log(msg *m);

int elaborate(msg *m)
{
	if (m->type == TYPE_EXIT)
	{
		cond_print("[Daemon:'Exit message recived']\n");
		return STOP_STATUS;
	}
	else
	{
		cond_print("[txt:%s , fn:%s]\n", m->msg_log.txt, m->msg_log.fn);
		return write_log(m);
	}
}

int write_log(msg *m)
{
	int res;
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
		return ERR_TYPE_NOT_SUPPORTED;
		break;
	}

	if (strlen(m->msg_log.txt) > 0)
	{
		f = fopen(m->msg_log.fn, "a");
		if (f == NULL)
		{
			//TODO: Gestisci errore
			fprintf(stderr, "Errore nell'apertura del file!: [%s]\n", m->msg_log.fn);
			return ERR_CANNOT_WRITE_FILE;
		}
		cond_print("[stampato]:%s\n", m->msg_log.fn);
		fprintf(f, "%s\n", m->msg_log.txt);
		fflush(f);
		fclose(f);
	}
	return OK_STATUS;
}