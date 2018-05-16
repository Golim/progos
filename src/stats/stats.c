#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <string.h>

#include "stats.h"

int mainaaa(int argc, char **argv)
{
	msg *m;
	char cmd[COMMAND_MAX_LEN];
	char sep[3];

	// Working var
	int i;
	bool valido;

	//Initialize default
	m = malloc(sizeof(msg));

	strcpy(m->msg_log.fn, "");
	strcpy(m->msg_log.txt, "");
	m->type = TYPE_CSV;

	strcpy(sep, ",");

	char *str = malloc(sizeof(char) * STATS_MAX_LEN);

	stats(cmd, str, sep);
	strcpy(m->msg_log.txt, str);
	send_to_logger(m);
	free(m);
}

void send_to_logger(msg *m)
{
	printf("\n ------------------------------------ \n | %li | %s | %s | -->\n ------------------------------------\n", m->type, m->msg_log.txt, m->msg_log.fn);
}

int stats(char *cmd, char *stat, char *sep)
{
	int codice_ritorno;
	long durata; //in ms
	char nome_comando[COMMAND_MAX_LEN];
	char argomenti[ARG_MAX_LEN];

	struct timespec clock_start;
	struct timespec clock_finish;

	separe_command_args(cmd, nome_comando, argomenti);

	// vedi altre opzioni su man clock_gettime per "tempi" diversi
	clock_gettime(CLOCK_REALTIME, &clock_start);
	codice_ritorno = system(cmd);
	clock_gettime(CLOCK_REALTIME, &clock_finish);

	durata = (clock_finish.tv_sec - clock_start.tv_sec) * 1000 + (clock_finish.tv_nsec - clock_start.tv_nsec) / TIME_FACTOR;

	sprintf(stat, "%s", nome_comando);
	strcat(stat, sep);
	strcat(stat, argomenti);
	strcat(stat, sep);
	sprintf(stat, "%s%lims", stat, durata);
	strcat(stat, sep);
	sprintf(stat, "%s%d", stat, codice_ritorno);

	return 0;
}

int separe_command_args(char *cmd, char *name, char *arg)
{
	int i = 0;

	char *tmp;
	//TODO: implementa caso specale, tipo PIPE etc...
	strcpy(arg, "");
	tmp = strtok(cmd, " ");
	strcpy(name, tmp);
	tmp = strtok(NULL, " ");
	while (tmp != NULL)
	{
		i++;
		if (strlen(arg) > 0)
			strcat(arg, " ");
		strcat(arg, tmp);
		tmp = strtok(NULL, " ");
	}
	return i;
}
