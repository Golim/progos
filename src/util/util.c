#include <stdarg.h>
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
