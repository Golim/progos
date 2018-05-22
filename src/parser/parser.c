#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#include "parser.h"
//Internal Fuctions
void add_token(exp_token t);
int is_meta_character(char c);
int tokenize(char *cmd);
int is_meta_character(char c);

//TODO: togli costanti
char controloperator[10][3];

exp_token tokens[N_MAX_TOKENS];
int tks = 0;

//int execute(int s, int f);

int run_cmd(char *cmd)
{
  tokenize(cmd);
  int n = execute(0, tks);
  return 0;
}

int separe_command_args(char *cmd, char *name, char *arg)
{
  int wc = 0;
  int i;
  int l = 0;

  int start_word = -1;
  int finish_word = -1;
  strcpy(name, "");
  strcpy(arg, "");

  for (i = 0; i <= strlen(cmd); i++)
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

int tokenize(char *cmd)
{
  int l;
  int i;
  exp_token et;
  char tok[255];
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

      if (strlen(controloperator[l]) > 0)
      {
        et.type = OPERATOR;
        strcpy(et.value, controloperator[l]);
        add_token(et);
        i += strlen(controloperator[l]) - 1;
      }
    }
  }
}

void add_token(exp_token t)
{
  tokens[tks] = t;
  tks++;
}

bool is_unary_operator(char *op)
{
  if (strcmp(op, ";;") == 0)
    return TRUE;
  if (strcmp(op, ";") == 0)
    return TRUE;
  if (strcmp(op, "\n") == 0)
    return TRUE;
  return FALSE;
}
//TODO: sistema l'array copiato ogni volta
int is_control_operator(char *cmd, int i)
{
  strcpy(controloperator[1], "||");
  strcpy(controloperator[2], "&&");
  strcpy(controloperator[3], "|&"); //TODO: impelmenta
  strcpy(controloperator[4], "&");
  strcpy(controloperator[5], ";;");
  strcpy(controloperator[6], ";");
  strcpy(controloperator[7], "(");
  strcpy(controloperator[8], ")");
  strcpy(controloperator[9], "|");
  strcpy(controloperator[0], "\n");
  int j;
  for (j = 0; j < 10; j++)
  {
    if (controloperator[j][0] == cmd[i])
    {
      if (strlen(controloperator[j]) == 1)
        return j;
      else if (controloperator[j][1] == cmd[i + 1])
        return j;
    }
  }
  return -1;
}

int is_meta_character(char c)
{
  char *metacharacter = "|&;() \t";
  if (index(metacharacter, c) == NULL)
    return -1;
  else
    return c;
}