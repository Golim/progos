#ifndef PARSER_h
#define PARSER_h

#define OPERATOR (1)
#define COMMAND (2)
#define N_MAX_TOKENS (20)

#include "../util/util.h"
#include "../execute/execute_commands.h"

/**
 * Divide the command in tokens and run it
 * Return the return code of execute function
 * */
int run_cmd(char *);

/**
 * Function extract the command name and the arduments from a string
 */
void separe_command_args(char *cmd, char *name, char *arg);

/**
 * A control operator is an operator that separe two command
*/
int is_control_operator(char *cmd, int i);

/**
 * Function that given a string that represent an operator return true if it exepect only one argument, typically used for terminate a simple command
 */
bool is_unary_operator(char *op);

typedef struct exp_token
{
  int type;
  char value[MAX_LEN_CMD];
} exp_token;


extern exp_token tokens[N_MAX_TOKENS];
extern int tks;  //number of token found

#endif