#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#include "parser.h"
//TODO: togli costanti
char controloperator[10][3];
exp_token tokens[50];

int tks = 0;

void add_token(exp_token t);
bool is_unary_operator(char *op);
int execute(int s, int f);
int execute_and(int s1, int f1, int s2, int f2);
int execute_or(int s1, int f1, int s2, int f2);
int execute_pipe(int s1, int f1, int s2, int f2);

extern int esegui_e_logga(char *);

int esegui_programma(char *cmd)
{
  parse(cmd);
  int n = execute(0, tks);
  return 0;
}

int parse(char *cmd)
{
  tokenize(cmd, 0, strlen(cmd) + 1);
}

int execute(int s, int f)
{
  //printf("%d %d \n", s, f);
  if (s == f)
  {
    if (strlen(tokens[s].value) > 0)
      return esegui_e_logga(tokens[s].value);
  }
  else if (s < f)
  {
    if (tokens[s].type == COMMAND)
    {
      int i = s + 1;
      if (tokens[i].type == COMMAND)
        return -1;
      if (is_unary_operator(tokens[i].value) == TRUE || s == tks - 1)
      {
        if (i + 1 >= f) //se sono oltre la fine eseguo solo la prima parte
          return execute(s, s);
        else
        {
          execute(s, s);
          return execute(i + 1, f);
        }
      }
      else // operatore binario
      {
        int j = i + 1;
        while (j <= f && !(is_unary_operator(tokens[i].value) == TRUE || j == tks - 1))
          j++;

        if (strcmp(tokens[i].value, "&&") == 0)
        {
          if (j == f && j + 1 <= tks)
            return execute_and(s, i - 1, i + 1, f);
          else
          {
            execute_and(s, i - 1, i + 1, j);
            return execute(j + 1, f);
          }
        }
        if (strcmp(tokens[i].value, "||") == 0)
        {
          if (j == f && j + 1 <= tks)
            return execute_or(s, i - 1, i + 1, f);
          else
          {
            execute_or(s, i - 1, i + 1, j);
            return execute(j + 1, f);
          }
        }
        if (strcmp(tokens[i].value, "|") == 0)
        {
          if (j == f && j + 1 <= tks)
            return execute_pipe(s, i - 1, i + 1, f);
          else
          {
            execute_pipe(s, i - 1, i + 1, j);
            return execute(j + 1, f);
          }
        }
      }
    }
  }
  return -1;
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

int tokenize(char *cmd, int s, int f)
{
  int l;
  int i;
  exp_token et;
  char tok[255];
  int start_word = -1;
  int finish_word = -2;

  int start_command = -1;
  int finish_command = -1;

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

/*
  execute 2 commands between and
  return: 
      -1 -> something went wrong
      0 -> everything ok
*/
int execute_and(int s1, int f1, int s2, int f2)
{
  if (execute(s1, f1) == 0)
    return execute(s2, f2);
  else
    return -1;
  return 0;
}

/*
  execute 2 commands between or
  return: 
      2 -> both commands went wrong
      0 -> at least one command run successfully
*/
int execute_or(int s1, int f1, int s2, int f2)
{
  // lazy evaluation
  if (execute(s1, f1) == 0)
    return 0;

  if (execute(s2, f2) == 0)
    return 0;

  return 2;
}

/* 
  execute 2 commands between pipe r
  Return: 
    -1 -> something went wrong
    0 -> everything ok
*/
int execute_pipe(int s1, int f1, int s2, int f2)
{
  //== create two process and make them comunicate using a pipe
  pid_t p;
  int pipefd[2];
  pipe(pipefd);
  int status;

  // save stdout and stdin
  int stdout_fd = dup(STDOUT_FILENO);
  int stdin_fd = dup(STDIN_FILENO);

  p = fork();
  if (p < 0)
    return -1;
  else if (p > 0)
  {
    //Esegui 1:
    dup2(pipefd[1], STDOUT_FILENO); // sposta stdout
    close(pipefd[0]);
    int r = execute(s1, f1); //esegui
    dup2(stdout_fd, 1);      //ripristina stdout
  }
  else
  {
    //Esegui 2
    dup2(pipefd[0], STDIN_FILENO); //Sposta stdin
    close(pipefd[1]);
    int s = execute(s2, f2); //esegui
    dup2(stdin_fd, 0);
    exit(s);
  }
  //restore stdin and stdout
  close(pipefd[0]);
  close(pipefd[1]);

  close(stdin_fd);
  close(stdout_fd);

  //wait for all child to end
  waitpid(-1, &status, 0);
  return status;
}

int execute_pipe_err(int s1, int f1, int s2, int f2)
{
  //TODO: implementa /&
  return FALSE;
}
