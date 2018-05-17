#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char controloperator[10][3];

exp_token tokens[50];
int tks = 0;
void add_token(exp_token t);

int main(int argc, char const *argv[])
{
  char *cmd = malloc(255);
  strcpy(cmd, argv[1]);
  system(argv[1]);
  parse(cmd);
  printf("\n");
  return 0;
}
int parse(char *cmd)
{
  tokenize(cmd, 0, strlen(cmd) + 1);

  for (int i = 0; i < tks; i++)
  {
    if (tokens[i].type == COMMAND)
    {
      printf("C:[%s]  ", tokens[i].value);
    }
    else if (tokens[i].type == OPERATOR)
    {
      printf("Op:[%s]  ", tokens[i].value);
    }
  }
}

int execute(int s, int finish)
{
  int i = s;
  if(i < finish)
  {

  }
}
int tokenize(char *cmd, int s, int f)
{
  int l;
  exp_token et;
  char tok[255];
  int start_word = -1;
  int finish_word = -2;

  int start_command = -1;
  int finish_command = -1;

  for (int i = s; i < f; i++)
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
      if (start_word > 0)
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
        i+=strlen(controloperator[l])-1;
      }
    }
  }
}

void add_token(exp_token t)
{
  tokens[tks] = t;
  tks++;
}

int is_control_operator(char *cmd, int i)
{
  strcpy(controloperator[1], "||");
  strcpy(controloperator[2], "&&");
  strcpy(controloperator[3], "|&");
  strcpy(controloperator[4], "&");
  strcpy(controloperator[5], ";;");
  strcpy(controloperator[6], ";");
  strcpy(controloperator[7], "(");
  strcpy(controloperator[8], ")");
  strcpy(controloperator[9], "|");
  strcpy(controloperator[0], "\n");

  for (int j = 0; j < 10; j++)
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
  if(index(metacharacter,c)== NULL)
    return -1;
  else
    return c;
}