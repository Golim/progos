#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <string.h>

#include "../util.h"
#include "stats.h"

int main(int argc, char **argv)
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

	if (argc < 2)
	{
		print_usage();
		exit(1);
	}
	else
	{
		// Il comando deve essere l'ultimo parametro.
		strcpy(cmd, argv[argc - 1]);
	}

	for (i = 1, valido = TRUE; i < argc && valido == TRUE; i++)
	{
		char *option = "";
		char *value = "";
		valido = FALSE;

		if (index(argv[i], '=') != NULL)
		{
			//TODO controlla che le opzioni non siano già state impostate
			//opzione=valore
			option = strtok(argv[i], "=");
			value = strtok(NULL, "");

			if (strcmp(option, "--format") == 0 || strcmp(option, "-f") == 0)
			{
				if (strcmp(value, "csv") == 0)
					valido = TRUE;
				else if (strcmp(value, "txt") == 0)
				{
					strcpy(sep, "\t");
					valido = TRUE;
					m->type = TYPE_TXT;
				}
				else
					valido = 0;
			}
			else if (strcmp(option, "--logfile") == 0 || strcmp(option, "-lf") == 0)
			{
				if (strcmp(value, "") != 0)
				{
					valido = TRUE;
					strcpy(m->msg_log.fn, value);
				}
			}
		}
		else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)
		{
			print_help();
			exit(0);
		}
		else if (i == argc - 1)
			valido = TRUE;
	}
	if (valido == FALSE)
	{
		//Errore nella lettura di un argomento.
		print_usage();
		exit(1);
	}

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

void print_usage()
{
	printf("Usage:  stats [OPTION]...[COMMAND]\n");
	printf("Try 'stats --help' for more information.\n");
}
void print_help()
{
	printf("Usage:  stats [OPTION]...[COMMAND]\n");
	printf("Execute COMMAND , log various statistics in a file\n");
	printf("The command MUST be the last argument passed to stats");
	printf("Example:  stats  ls \n");
	printf("\n");

	printf("-OPTION\n");
	printf("All OPTION are not mandatory. Contemplated options are:\n");
	printf(" -h \t \t--help \t\t print this message\n");
	printf(" -lf=[v]\t--logfile=[v]\t where [v] specifies the log file where stats will be appended\n");
	printf(" -f=[v] \t--format=[v]\t specifies the format for the output. [v] can be 'csv' or 'txt'\n");
	printf("\n");
}