#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>

#include "../util.h"
#include "stats.h"

int main(int argc, char **argv)
{
	msg *m;
	char cmd[COMMAND_MAX_LEN];
	char sep[SEP_MAX_LEN];

	// Working var
	int i;
	bool valido;

	//Initialize default
	m = malloc(sizeof(msg));

	strcpy(m->msg_log.fn, "");
	strcpy(m->msg_log.txt, "");
	m->type = TYPE_CSV;

	strcpy(sep, ",");

	bool mu = TRUE;
	bool names = FALSE;

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

			tolowercase(value);

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
			else if (strcmp(option, "--measure-units") == 0 || strcmp(option, "-mu") == 0)
			{
				if (strcmp(value, "false") == 0)
				{
					mu = FALSE;
					valido = TRUE;
				}
				else if (strcmp(value, "true") == 0)
				{
					mu = TRUE;
					valido = TRUE;
				}
			}
			else if (strcmp(option, "--separator") == 0 || strcmp(option, "-s") == 0)
			{
				if (strcmp(value, "") != 0)
				{
					strcpy(sep, value);
					valido = TRUE;
				}
			}
		}
		else if (strcmp(argv[i], "--names") == 0 || strcmp(argv[i], "-n") == 0)
		{
			names = TRUE;
			valido = TRUE;
		}
		else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)
		{
			print_help();
			exit(0);
		}
		else if (strcmp(argv[i], "--usage") == 0 || strcmp(argv[i], "-u") == 0)
		{
			print_usage();
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

	stats(cmd, str, sep, mu, names);
	strcpy(m->msg_log.txt, str);
	send_to_logger(m);
	free(m);
}

void send_to_logger(msg *m)
{
	printf("\n\
 ____________________________________________________________________________________________________________________________\n\
 |msg_type| %li\n\
 |log  msg| %s\n\
 |log file| %s\n\
 ____________________________________________________________________________________________________________________________\n",
				 m->type, m->msg_log.txt, m->msg_log.fn);
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

void tolowercase(char *s)
{
	for (int i = 0; s[i]; i++)
	{
		if ('A' <= s[i] && s[i] <= 'Z')
			s[i] = s[i] - ('A' - 'a');
	}
}

void print_usage()
{
	printf("Usage:  stats [OPTION]... [COMMAND]\n");
	printf("Try 'stats --help' for more information.\n");
}

void print_help()
{
	printf("\
Usage:  stats [OPTION]... [COMMAND]\n\
Execute COMMAND , log various statistics in a file\n\
The command MUST be the last argument passed to stats\n\
Example:  stats  ls \n\
If the command has argument, the command has to be in quotes\n\
Example:  stats \"ls -al\"\n\n");

	printf("-OPTION\n");
	printf("All OPTION are not mandatory. Contemplated options are:\n");

	//-f --format
	printf("%10s", "-f=[v], ");
	printf("%-20s", "--format=[v]");
	printf("%s", "specifies the format for the output.\n\
                               [v] can be 'csv' or 'txt'\n"); //Aggiungere default="txt"(??)

	//-lf --logfile
	printf("%10s", "-lf=[v], ");
	printf("%-20s", "--logfile=[v]");
	printf("%s", "where [v] specifies the log file where stats will be appended\n");

	//-mu --measure-unit
	printf("%10s", "-mu=[v], ");
	printf("%-20s", "-measure-units=[v]");
	printf("%s", "specifies whether the output should contain the unit of measurement.\n\
                               [v] can be 'true' or 'false'\n");

	//-n --names
	printf("%10s", "-n, ");
	printf("%-20s", "--names");
	printf("%s", "specifies whether the output should contain the names of the fields.\n");

	//-h --help
	printf("%10s", "-h, ");
	printf("%-20s", "--help");
	printf("%s", "display this help and exit\n");

	//-s --separator
	printf("%10s", "-s=[v], ");
	printf("%-20s", "--separator=[v]");
	printf("%s", "specifies the separator that appears between the fields.\n\
                               [v] can be a character or a string \n\
                               in quotes if it is composed only of spaces \n\
                               or if it contains special characters (|, &, ;, (, ) ...).\n\
                               e.g. -s=\" | \"\n"); //Definire meglio gli special characters

	//-u --usage
	printf("%10s", "-u, ");
	printf("%-20s", "--usage");
	printf("%s", "give a short usage message\n");

	printf("\n");
}
