#ifndef UTIL_h
#define UTIL_h

#define MAXLEN 255

#define MAX_LEN_FN (255)
#define MAX_LEN_CMD (255)
#define MAX_LEN_STAT (500)

#define TRUE 1
#define FALSE 0

#define DEF_TXT ("/tmp/log.txt")
#define DEF_CSV ("/tmp/log.csv")

#define TYPE_CSV (1)
#define TYPE_TXT (2)
#define TYPE_EXIT (3)

#define ARG_TOO_FEW (-1)
#define ARG_NOT_VALID_CMD (-2)
#define ARG_DUP (-3)
#define ARG_NOT_VALID_FN (-4)
#define ARG_BAD_USAGE (-5)

#define ERR_TYPE_NOT_SUPPORTED (-2)
#define ERR_CANNOT_WRITE_FILE (-1)

#define OK_STATUS (0)
#define FAIL_STATUS (-1)
#define STOP_STATUS (10)

#define UNSET (-2147483648)

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

char *btoa(bool i);
char *ftoa(bool i);
int cond_print(const char *format, ...);

#endif