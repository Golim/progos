#ifndef UTIL_h
#define UTIL_h


#define MAX_LEN_FN    (255)
#define MAX_LEN_CMD   (255)
#define MAX_LEN_STAT  (500)
#define MAX_LEN_SEP   ( 20)

#define TRUE 1
#define FALSE 0

#define DEF_TXT ("/tmp/log.txt")
#define DEF_CSV ("/tmp/log.csv")

#define TYPE_CSV  (1)
#define TYPE_TXT  (2)
#define TYPE_EXIT (3)

#define ARG_TOO_FEW           (-11)
#define ARG_NOT_VALID_CMD     (-12)
#define ARG_DUP               (-13)
#define ARG_NOT_VALID_FN      (-14)
#define ARG_BAD_USAGE         (-15)
#define ARG_NOT_SUPPORTED_CMD (-16)
#define ARG_TOO_LONG      (-17)

#define ERR_TYPE_NOT_SUPPORTED  (-2)
#define ERR_CANNOT_WRITE_FILE   (-1)

#define ERR_WAIT    (-20)
#define ERR_WAITPID (-21)
#define ERR_DUP     (-22)
#define ERR_DUP2    (-23)
#define ERR_PIPE    (-24)
#define ERR_OPEN    (-25)
#define ERR_CLOSE   (-26)
#define ERR_REMOVE  (-27)

#define OK_STATUS   ( 0)
#define FAIL_STATUS (-1)
#define STOP_STATUS (10)

#define UNSET (-2147483648)

//white color for cmd
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define BOLDWHITE   "\033[1m\033[37m"

typedef int bool;
typedef char *string;

struct log
{
  char txt[MAX_LEN_STAT];
  char fn[MAX_LEN_FN];
};

typedef struct msg
{
  long type;
  struct log msg_log;
} msg;

char *btoa(bool i);
char *ftoa(bool i);
int cond_print(const char *format, ...);
bool is_valid_filename(char *fn);
int is_valid_command(char *cmd);

#endif