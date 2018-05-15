#define MAXLEN 255

#define TRUE 1
#define FALSE 0

#define TYPE_CSV (1)
#define TYPE_TXT (2)
#define TYPE_EXIT (3)

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
