#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <string.h>

const long TIME_FACTOR = 1000000;
const long COMMAND_MAX_LEN = 255;
const long STATS_MAX_LEN = 255;
const long ARG_MAX_LEN = 255;

int stats(char *cmd, char *stat);
int separe_command_args(char* cmd, char * name, char* arg);
int main(int argc, char **argv)
{
	char * str = malloc (sizeof(char)*STATS_MAX_LEN);
	printf("[Inizio]\n");

	stats(argv[1], str);

	printf("[%s]\n", str);
}

int stats(char *cmd, char *stat)
{
	int codice_ritorno;
	long durata; //in ms
	char nome_comando[COMMAND_MAX_LEN];
	char argomenti [ARG_MAX_LEN];

	struct timespec clock_start;
	struct timespec clock_finish;

	separe_command_args(cmd,nome_comando,argomenti);

	printf("Cmd:[%s]\n", cmd);

	// vedi altre opzioni su man clock_gettime per "tempi" diversi
	clock_gettime(CLOCK_REALTIME, &clock_start);
	codice_ritorno = system(cmd);
	clock_gettime(CLOCK_REALTIME, &clock_finish);

	durata = (clock_finish.tv_sec - clock_start.tv_sec )*1000 + (clock_finish.tv_nsec - clock_start.tv_nsec) / TIME_FACTOR;

	sprintf(stat, "%s , %s , %li , %i", nome_comando, argomenti, durata , codice_ritorno );

	return 0;
}

int separe_command_args(char* cmd, char * name, char* arg)
{
	int i = 0;

	char* tmp;

	strcpy(arg, "");
	tmp = strtok(cmd," ");
	strcpy(name,tmp);
	tmp = strtok(NULL," ");
	while(tmp != NULL)
	{
		i++;
		if(strlen(arg)> 0)
			strcat(arg, " ");
		strcat(arg, tmp);
		tmp = strtok(NULL," ");
	}
	return i;
}