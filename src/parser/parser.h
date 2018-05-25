#ifndef PARSER_h
#define PARSER_h

#define OPERATOR (1)
#define COMMAND (2)
#define N_MAX_TOKENS (20)

#include "../util/util.h"
#include "../execute/execute_commands.h"

int run_cmd(char *);

int separe_command_args(char *cmd, char *name, char *arg);
int is_control_operator(char *cmd, int i);
bool is_unary_operator(char *op);

typedef struct exp_token
{
  int type;
  char value[MAX_LEN_CMD];
} exp_token;


extern exp_token tokens[N_MAX_TOKENS];
extern int tks;  //number of token found

#endif