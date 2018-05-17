#ifndef UTIL_h
#define UTIL_h

#define MAXLEN 255


#define MAX_LEN_FN (255)
#define MAX_LEN_CMD (255)
#define MAX_LEN_STAT (500)

#define TRUE 1
#define FALSE 0

#define TYPE_CSV (1)
#define TYPE_TXT (2)
#define TYPE_EXIT (3)

#define ARG_TOO_FEW (-1)
#define ARG_NOT_VALID_CMD (-2)
#define ARG_DUP (-3)
#define ARG_NOT_VALID_FN (-4)
#define ARG_BAD_USAGE (-5)

#define OK_STATUS (0)

#define UNSET (-2147483648)

#include <stdarg.h>

typedef int bool;
typedef char *string;

struct log
{
  char txt[255];
  char fn[255];
};

typedef struct msg
{
  long type;
  struct log msg_log;
} msg;

extern bool verbose;
extern int cond_print(const char *format, ...);
extern char *btoa(bool i);
extern char *ftoa(bool i);
#endif