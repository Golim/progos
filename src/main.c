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
int cond_print(const char *format, ...);

// CONFIG
int format = UNSET;
int verbose = UNSET;
bool mu = UNSET;
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

  cond_print("Argomenti letti con successo\n");
  cond_print("Eseguzione programma con: \n -format: %s \n -filename: %s\n -cmd: %s \n -mu: %s\n", ftoa(format), filename, cmd, btoa(mu));
  run(format, filename, cmd, mu);
  return 0;
}
char *btoa(bool i)
{
  return i == TRUE ? "true" : "false";
}
char *ftoa(bool i)
{
  if (i == TYPE_CSV)
    return "csv";
  else if (i == TYPE_TXT)
    return "txt";
  else if (i == TYPE_EXIT)
    return "exit";
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
  int i;
  for (i = 1; i < argc && valido == TRUE; i++)
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
      else if (strcmp(option, "-measure-units") == 0 || strcmp(option, "-mu") == 0)
      {
        if (mu != UNSET)
          return ARG_DUP;

        if (strcmp(value, "true") == 0)
        {
          valido = TRUE;
          mu = TRUE;
        }
        else if (strcmp(value, "false") == 0)
        {
          valido = TRUE;
          mu = FALSE;
        }
      }
    }
    else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)
    {
      print_help();
      exit(EXIT_SUCCESS);
    }
    else if (strcmp(argv[i], "--verbose") == 0 || strcmp(argv[i], "-v") == 0)
    {
      if (verbose != UNSET)
      {
        return ARG_DUP;
      }
      else
      {
        verbose = TRUE;
        valido = TRUE;
      }
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
    if (verbose == UNSET)
      verbose = FALSE;
    if (mu == UNSET)
      mu = TRUE;
    if (arg_filename == UNSET)
      if (format == TYPE_CSV)
        strcpy(filename, DEF_CSV);
      else if (format == TYPE_TXT)
        strcpy(filename, DEF_TXT);

    arg_filename = TRUE;
  }
  return OK_STATUS;
}
void print_usage()
{
  printf("Usage:  stats [OPTION]... [COMMAND]\n");
  printf("Try 'stats --help' for more information.\n");
}

void print_help()
{
  printf("Usage:  stats [OPTION]... [COMMAND]\n");
  printf("Execute COMMAND , log various statistics in a file\n");
  printf("The command MUST be the last argument passed to stats");
  printf("Example:  stats  ls \n"); //Aggiungere che se ci sono anche argomenti il comando va passato tra virgolette
  printf("\n");

  printf("-OPTION\n");
  printf("All OPTION are not mandatory. Contemplated options are:\n");

  //-f --format
  printf("%10s", "-f=[v], ");
  printf("%-20s", "--format=[v]");
  printf("%s", "specifies the format for the output. [v] can be 'csv' or 'txt'\n"); //Aggiungere default="txt"(??)

  //-lf --logfile
  printf("%10s", "-lf=[v], ");
  printf("%-20s", "--logfile=[v]");
  printf("%s", "where [v] specifies the log file where stats will be appended\n");

  //-mu --measure-unit
  printf("%10s", "-mu=[v], ");
  printf("%-20s", "-measure-units=[v]");
  printf("%s", "specifies whether the output should contain the unit of measurement. [v] can be 'true' or 'false'\n");

  //-v --verbose
  printf("%10s", "-v, ");
  printf("%-20s", "--verbose");
  printf("%s", "specifies whether to print verbose output'\n");

  //-h --help
  printf("%10s", "-h, ");
  printf("%-20s", "--help");
  printf("%s", "display this help and exit\n");

  printf("\n");
}

int cond_print(const char *format, ...)
{
  if (verbose == TRUE)
  {
    va_list args;
    va_start(args, format);
    int ret = vprintf(format, args);
    va_end(args);
    return ret;
  }
  return 0;
}