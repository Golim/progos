#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <errno.h>

#include "./util/util.h"
#include "config_output.h"
#include "./parser/parser.h"

void print_usage();
void print_help();

int parse_argument(int argc, char **argv);
void set_config_defaults();
int cond_print(const char *format, ...);

char cmd[MAX_LEN_CMD] = "";

bool arg_filename = UNSET;
char filename[MAX_LEN_FN] = "";
bool names = UNSET;
int arg_sep = UNSET;
char sep[10] = "";
int format = UNSET;
int verbose = UNSET;
bool mu = UNSET;

int main(int argc, char **argv)
{
  int r;
  r = parse_argument(argc, argv);
  if (r < 0)
  {
    fprintf(stderr, "Error while reading arguments. Error Code:[%d]\n", r);
    print_usage();
    exit(r);
  }

  cond_print("[Running program with parameters]: \n -format: %s \n -filename: %s\n -cmd: %s \n -mu: %s\n -sep: %s \n", ftoa(format), filename, cmd, btoa(mu), sep);
  run_program(cmd);
  return 0;
}

int parse_argument(int argc, char **argv)
{
  int i;

  bool valido = TRUE;
  char *option = "";
  char *value = "";

  //Troppi pochi argomenti
  if (argc < 2)
    return ARG_TOO_FEW;
  if (is_valid_command(argv[argc - 1]))
    strcpy(cmd, argv[argc - 1]);

  for (i = 1; i < argc && valido == TRUE; i++)
  {
    valido = FALSE;

    if (index(argv[i], '=') != NULL)
    {
      option = strtok(argv[i], "=");
      value = strtok(NULL, "");
      if (option != NULL && value != NULL)
      {
        if (strcmp(option, "--format") == 0 || strcmp(option, "-f") == 0)
        {
          if (format != UNSET)
            return ARG_DUP;

          if (arg_sep != UNSET)
            return ARG_BAD_USAGE;

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
        else if (strcmp(option, "--separator") == 0 || strcmp(option, "-s") == 0)
        {
          if (arg_sep != UNSET)
            return ARG_DUP;

          if (format != UNSET)
            return ARG_BAD_USAGE;

          else if (strcmp(value, "") != 0)
          {
            strcpy(sep, value);
            format = TYPE_TXT;
            arg_sep = TYPE_TXT;
          }
          valido = TRUE;
        }
      }
    }

    else if (strcmp(argv[i], "--names") == 0 || strcmp(argv[i], "-n") == 0)
    {
      if (names != UNSET)
        return ARG_DUP;
      else
      {
        names = TRUE;
        valido = TRUE;
      }
    }
    else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)
    {
      print_help();
      exit(EXIT_SUCCESS);
    }
    else if (strcmp(argv[i], "--usage") == 0 || strcmp(argv[i], "-u") == 0)
    {
      print_usage();
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
  if (strlen(cmd) == 0)
    return ARG_NOT_VALID_CMD;
  set_config_defaults();
  return OK_STATUS;
}
void print_usage()
{
  printf("Usage:  stats [OPTION]... [COMMAND]\n");
  printf("Try 'stats --help' for more information.\n");
}

void set_config_defaults()
{
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

  if (arg_sep == UNSET)
    if (format == TYPE_CSV)
      strcpy(sep, " , ");
    else if (format == TYPE_TXT)
      strcpy(sep, "\t");

  if (names == UNSET)
    names = FALSE;

  char *abs_path = NULL;
  abs_path = realpath(filename, NULL);
  if (abs_path != NULL && strlen(abs_path) > 0)
    strcpy(filename, abs_path);

  free(abs_path);
  arg_sep = TRUE;
  arg_filename = TRUE;
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
