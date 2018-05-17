#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "stats.h"

int statos(char *cmd, char *stat, char *sep, bool mu, bool names)
{
	printf("a: '%s' \n\n",cmd);
}
int stats(char *cmd, char *stat, char *sep, bool mu, bool names)
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

	if (names == TRUE)
		sprintf(stat, "%s[cmd]", stat);
	sprintf(stat, "%s%s", stat, nome_comando);
	strcat(stat, sep);
	if (names == TRUE)
		sprintf(stat, "%s[args]", stat);
	strcat(stat, argomenti);
	strcat(stat, sep);

	if (names == TRUE)
		sprintf(stat, "%s[duration_realtime]", stat);
	if (mu == TRUE)
		sprintf(stat, "%s%lims", stat, durata_realtime);
	else
		sprintf(stat, "%s%li", stat, durata_realtime);

	strcat(stat, sep);

	if (names == TRUE)
		sprintf(stat, "%s[duration_CPU]", stat);
	if (mu == TRUE)
		sprintf(stat, "%s%lius", stat, durata_cputime);
	else
		sprintf(stat, "%s%li", stat, durata_cputime);

	strcat(stat, sep);
	if (names == TRUE)
		sprintf(stat, "%s[return_code]", stat);
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
