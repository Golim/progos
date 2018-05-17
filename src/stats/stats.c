#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>

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

	stats(cmd, str, sep, TRUE);
	strcpy(m->msg_log.txt, str);
	send_to_logger(m);
	free(m);
}

void send_to_logger(msg *m)
{
	printf("\n ------------------------------------ \n | %li | %s | %s | -->\n ------------------------------------\n", m->type, m->msg_log.txt, m->msg_log.fn);
}

int stats(char *cmd, char *stat, char *sep, bool mu) //mu: stampa unità di misura se true
{
	int codice_ritorno;
	long durata_realtime; //in ms
	long durata_cputime;	//in ms
	char nome_comando[COMMAND_MAX_LEN];
	char argomenti[ARG_MAX_LEN];

	struct timespec clock_start_realtime;
	struct timespec clock_start_cputime;

	struct timespec clock_finish_realtime;
	struct timespec clock_finish_cputime;

	separe_command_args(cmd, nome_comando, argomenti);

	clock_gettime(CLOCK_REALTIME, &clock_start_realtime);					 //Inizio realtime
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &clock_start_cputime); //Inizio CPUtime
	//Esecuzione comando
	codice_ritorno = system(cmd);

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &clock_finish_cputime);
	clock_gettime(CLOCK_REALTIME, &clock_finish_realtime);

	//Durata real-time
	durata_realtime = (clock_finish_realtime.tv_sec - clock_start_realtime.tv_sec) * 1000 + (clock_finish_realtime.tv_nsec - clock_start_realtime.tv_nsec) / TIME_FACTOR;
	//Durata CPU-time
	durata_cputime = (clock_finish_cputime.tv_sec - clock_start_cputime.tv_sec) * 1000000 + (clock_finish_cputime.tv_nsec - clock_start_cputime.tv_nsec) / 1000;

	sprintf(stat, "%s", nome_comando);
	strcat(stat, sep);
	strcat(stat, argomenti);
	strcat(stat, sep);

	if (mu == TRUE)
		sprintf(stat, "%s%lims", stat, durata_realtime);
	else
		sprintf(stat, "%s%li", stat, durata_realtime);

	strcat(stat, sep);

	if (mu == TRUE)
		sprintf(stat, "%s%lius", stat, durata_cputime);
	else
		sprintf(stat, "%s%li", stat, durata_cputime);

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

void tolowercase(char *s)
{
	for (int i = 0; s[i]; i++)
	{
		if ('A' <= s[i] && s[i] <= 'Z')
			s[i] = s[i] - ('A' - 'a');
	}
}