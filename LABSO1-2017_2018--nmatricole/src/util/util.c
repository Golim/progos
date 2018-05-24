#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util.h"
#include "../config_output.h"

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
  else
    return "N.D.";
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

/**
 * check if the file exists
 * Return: 
 *  - TRUE if the file exists
 *  - FALSE if the file not exists
 * */
bool is_valid_filename(char *fn)
{
  if (fn != NULL && strlen(fn) > 0)
  {
    FILE *f = fopen(fn, "a+");
    if (f == NULL)
      return FALSE;
    else
    {
      fclose(f);
      return TRUE;
    }
  }
  else
    return FALSE;
}

/**
 * check if the is tring is a command or a logical connector
 * Return: 
 *  - TRUE if the string is a command
 *  - FALSE if the string is a connector
 * */
int is_valid_command(char *cmd)
{
  if (strstr(cmd, "(") != NULL)
    return FALSE;
  if (strstr(cmd, ")") != NULL)
    return FALSE;
  if (strstr(cmd, " & ") != NULL)
    return FALSE;
  if (strstr(cmd, "[") != NULL)
    return FALSE;
  if (strstr(cmd, "]") != NULL)
    return FALSE;
  return TRUE;
}