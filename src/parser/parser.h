#ifndef PARSER_h
#define PARSER_h

#define OPERATOR (1)
#define COMMAND (2)
#define OPERATOR (1)

#include "../util/util.h"

int esegui_programma(char *);

int parse(char *cmd);
int separe_command_args(char *cmd, char *name, char *arg);
int tokenize(char *cmd, int start, int finish);
int is_control_operator(char *cmd, int i);
int is_meta_character(char c);

//TODO: impelmenta is_valid_command con gli operatori gestiti.
int is_valid_command(char *cmd);
bool is_valid_filename(char *fn);
typedef struct exp_token
{
  int type;
  char value[MAX_LEN_CMD];
} exp_token;

#endif