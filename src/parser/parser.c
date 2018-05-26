#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "parser.h"
//Internal Fuctions
void add_token(exp_token t);
int is_meta_character(char c);
void tokenize(char *cmd);
int is_meta_character(char c);

const int N_CONTROL_OPERATOR = 11;
const char *controloperators[] = {"\n", "||", "&&", "|&", "&", ";;", ";", "(", ")", "|", "\0"};


// Global variables also used by module "execute"
exp_token tokens[N_MAX_TOKENS];
int tks = 0;


int run_cmd(char *cmd)
{
  tokenize(cmd);
  int n = execute(0, tks);
  return n;
}

/*
* Function extract the command name and the arduments from a string
*/
void separe_command_args(char *cmd, char *name, char *arg)
{
  int wc = 0;
  int i;
  int l = 0;

  int start_word = -1;
  int finish_word = -1;
  strcpy(name, "");
  strcpy(arg, "");

  for (i = 0; i <= abs(strlen(cmd)); i++)
  {
    if (is_meta_character(cmd[i]) == -1)
    {
      //NON é UN SEPARATORE
      if (start_word < 0)
        start_word = i;
    }
    else
    {
      if (start_word >= 0)
      {
        finish_word = i;
        if (wc == 0)
        {
          //la prima parola è il nome comando
          strncpy(name, cmd + start_word, finish_word - start_word);
          name[finish_word - start_word] = '\0';
        }
        else
        {
          //sono argomenti
          if (wc > 1)
            strcat(arg, " ");

          l = strlen(arg);
          strcat(arg, cmd + start_word);
          arg[l + (finish_word - start_word)] = '\0';
        }
        start_word = -1;
        finish_word = -1;
        wc++;
      }
    }
  }
}

/*
  Function that given a string that rapresent a comand (simple or composite)
  subdivite it in several token. Each token represent a simple command or an operator.
  The token sequence is stored in global variable "tokens"
*/
void tokenize(char *cmd)
{
  int l;
  int i;
  exp_token et;
  char tok[MAX_LEN_CMD];
  int start_word = -1;
  int finish_word = -2;

  int start_command = -1;
  int finish_command = -1;
  int s = 0;
  int f = strlen(cmd) + 1;
  for (i = s; i < f; i++)
  {
    if (is_meta_character(cmd[i]) == -1)
    {
      //NON é UN SEPARATORE
      if (start_word < 0)
        start_word = i;
      if (start_command < 0)
        start_command = i;
    }
    else
    {
      if (start_word >= 0)
      {
        finish_word = i;
        strncpy(tok, cmd + start_word, finish_word - start_word);
        tok[finish_word - start_word] = '\0';
        start_word = -1;
        finish_word = -1;
      }
    }
    if ((l = is_control_operator(cmd, i)) >= 0 || cmd[i] == '\0')
    {
      //FINITO UN COMANDO
      finish_command = i;

      strncpy(tok, cmd + start_command, finish_command - start_command);
      tok[finish_command - start_command] = '\0';
      if (strlen(tok) > 0)
      {
        et.type = COMMAND;
        strcpy(et.value, tok);
        add_token(et);
      }
      start_command = -1;
      finish_command = -1;
      if (cmd[i] == '\0')
      {
        et.type = OPERATOR;
        strcpy(et.value, "\0");
        add_token(et);
      }
      else
      if (strlen(controloperators[l]) > 0)
      {
        et.type = OPERATOR;
        strcpy(et.value, controloperators[l]);
        add_token(et);
        i += strlen(controloperators[l]) - 1;
      }
    }
  }
}

//support function to add a token to "tokens" and update the number of tokens: tks
void add_token(exp_token t)
{
  tokens[tks] = t;
  tks++;
}
/**
 * Function that given a string that represent an operator return true if it exepect only one argument, typically used for terminate a simple command
 */
bool is_unary_operator(char *op)
{
  if (strcmp(op, ";;") == 0)
    return TRUE;
  if (strcmp(op, ";") == 0)
    return TRUE;
  if (strcmp(op, "\n") == 0)
    return TRUE;
  if (strcmp(op, "\0") == 0)
    return TRUE;
  return FALSE;
}

/**
 * A controll operator is an operator that separe two command
*/
int is_control_operator(char *cmd, int i)
{
  int j;
  for (j = 0; j < N_CONTROL_OPERATOR; j++)
  {
    if (controloperators[j][0] == cmd[i])
    {
      if (strlen(controloperators[j]) == 1)
        return j;
      else if (controloperators[j][1] == cmd[i + 1])
        return j;
    }
  }
  return -1;
}

/**
 * A meta character is a character that separes "word": some of them are used to separe arguments passed to a command (space, tab) other as operators.
*/
int is_meta_character(char c)
{
  char *metacharacter = "|&;() \t";
  if (index(metacharacter, c) == NULL)
    return -1;
  else
    return c;
}