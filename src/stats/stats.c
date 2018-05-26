#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "stats.h"
#include "../parser/parser.h"

/**
 * calcoulate the statistics of a single command with one or more parameters
 * Return: the result code of the execution of the received command
 * */
int stats(char *cmd, char *stat, char *sep, bool mu, bool names)
{
	strcpy(stat,"");
	int return_code;
	long realtime_length; //in ms
	long cputime_length;  //in ms
	char nome_comando[COMMAND_MAX_LEN];
	char argomenti[ARG_MAX_LEN];

	struct timespec clock_start_realtime;
	struct timespec clock_start_cputime;

	struct timespec clock_finish_realtime;
	struct timespec clock_finish_cputime;

	separe_command_args(cmd, nome_comando, argomenti);

	clock_gettime(CLOCK_REALTIME, &clock_start_realtime);		 //Start realtime
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &clock_start_cputime); //Start CPUtime
	
	//Cambia la cartella di lavoro del processo se il comando contiene cd
	printf("%s, \"%s\"\n", nome_comando, argomenti);
	if(strcmp(nome_comando, "cd") == 0){
		if(strlen(argomenti) == 0)
			strcpy(argomenti, getenv("HOME"));
		chdir(argomenti);
	}

	//Execute command
	return_code = system(cmd);

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &clock_finish_cputime);
	clock_gettime(CLOCK_REALTIME, &clock_finish_realtime);

	//Get real-time length
	realtime_length = (clock_finish_realtime.tv_sec - clock_start_realtime.tv_sec) * 1000 + (clock_finish_realtime.tv_nsec - clock_start_realtime.tv_nsec) / TIME_FACTOR;
	//Get CPU-time length
	cputime_length = (clock_finish_cputime.tv_sec - clock_start_cputime.tv_sec) * 1000000 + (clock_finish_cputime.tv_nsec - clock_start_cputime.tv_nsec) / 1000;

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
		sprintf(stat, "%s%lims", stat, realtime_length);
	else
		sprintf(stat, "%s%li", stat, realtime_length);

	strcat(stat, sep);

	if (names == TRUE)
		sprintf(stat, "%s[duration_CPU]", stat);
	if (mu == TRUE)
		sprintf(stat, "%s%lius", stat, cputime_length);
	else
		sprintf(stat, "%s%li", stat, cputime_length);

	strcat(stat, sep);
	if (names == TRUE)
		sprintf(stat, "%s[return_code]", stat);
	sprintf(stat, "%s%d", stat, return_code);

	return return_code;
}
