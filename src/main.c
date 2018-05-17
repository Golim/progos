#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "./util.h"
#include "./program/program.h"
#include "./parser/parser.h"
#include "./logger/logger.h"

void print_usage();
void print_help();

int parse_argument(int argc, char **argv);

// CONFIG
int format = UNSET;
char filename[MAX_LEN_FN] = "";
int arg_filename = UNSET;
char cmd[MAX_LEN_CMD] = "";

int main(int argc, char **argv)
{
  int r;
  r = parse_argument(argc, argv);
  if (r < 0)
  {
    fprintf(stderr, "Errore nella lettura degli argomenti. Codice uscita:[%d]\n", r);
    print_usage();
    exit(r);
  }

  run(format, filename, cmd);
  return 0;
}

int parse_argument(int argc, char **argv)
{
  bool valido = TRUE;
  char *option = "";
  char *value = "";

  //Troppi pochi argomenti
  if (argc < 2)
  {
    return ARG_TOO_FEW;
  }

  if (is_valid_command(argv[argc - 1]) < 0)
    return ARG_NOT_VALID_CMD;
  else
  {
    valido = TRUE;
    strcpy(cmd, argv[argc - 1]);
  }

  valido = TRUE;
  for (int i = 1; i < argc && valido == TRUE; i++)
  {
    valido = FALSE;

    if (index(argv[i], '=') != NULL)
    {
      option = strtok(argv[i], "=");
      value = strtok(NULL, "");

      if (strcmp(option, "--format") == 0 || strcmp(option, "-f") == 0)
      {
        if (format != UNSET)
          return ARG_DUP;

        if (strcmp(value, "csv") == 0)
        {
          valido = TRUE;
          format = TYPE_CSV;
        }
        else if (strcmp(value, "txt") == 0)
        {
          valido = TRUE;
          format = TYPE_TXT;
        }
      }
      else if (strcmp(option, "--logfile") == 0 || strcmp(option, "-lf") == 0)
      {
        if (arg_filename != UNSET)
          return ARG_DUP;

        arg_filename = TRUE;

        if (is_valid_filename(value) == FALSE)
        {
          return ARG_NOT_VALID_FN;
        }
        else
        {
          strcpy(filename, value);
          valido = TRUE;
        }
      }
    }
    else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)
    {
      print_help();
      exit(EXIT_SUCCESS);
    }
    else if (i == argc - 1)
    {
      if (is_valid_command(cmd) == TRUE)
        valido = TRUE;
    }
    else
      valido = FALSE;
  }

  if (valido == FALSE)
  {
    return ARG_BAD_USAGE;
  }
  else
  {
    //Initialize defaults:
    if (format == UNSET)
      format = TYPE_CSV;
    if (arg_filename == UNSET)
      if (format == TYPE_CSV)
        strcpy(filename, DEF_CSV);
      else if (format == TYPE_TXT)
        strcpy(filename, DEF_TXT);

    arg_filename = TRUE;
  }
  return OK_STATUS;
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

void print_usage()
{
  printf("Usage:  stats [OPTION]...[COMMAND]\n");
  printf("Try 'stats --help' for more information.\n");
}
