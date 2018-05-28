#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "execute_commands.h"

//internal file functions
int execute_and(int s1, int f1, int s2, int f2);
int execute_or(int s1, int f1, int s2, int f2);
int execute_pipe(int s1, int f1, int s2, int f2);
int execute_pipe_err(int s1, int f1, int s2, int f2);

extern int run_and_log(char *);

/**
 * execute a command formatted as a list of tokens
 * Return the return value of run_and_log function if everything works fine
 *        -1 otherwise
 * */
int execute(int s, int f)
{
  if (s == f)
  {
    if (strlen(tokens[s].value) > 0)
      return run_and_log(tokens[s].value);
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
        if (i + 1 >= f) // if the index is over the end, execute only the first part
          return execute(s, s);
        else
        {
          execute(s, s);
          return execute(i + 1, f);
        }
      }
      else // binary operator
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
        if (strcmp(tokens[i].value, "|&") == 0)
        {
          if (j == f && j + 1 <= tks)
            return execute_pipe_err(s, i - 1, i + 1, f);
          else
          {
            execute_pipe_err(s, i - 1, i + 1, j);
            return execute(j + 1, f);
          }
        }
      }
    }
  }
  return -1;
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
  execute 2 commands between pipe 
  Return: 
    -1 -> something went wrong
    0 -> everything ok
*/
int execute_pipe(int s1, int f1, int s2, int f2)
{
  //**create two process and make them comunicate using a pipe**
  pid_t p;
  int pipefd[2];
  my_pipe(pipefd);
  int status;

  // save stdout and stdin
  int stdout_fd = my_dup(STDOUT_FILENO);
  int stdin_fd = my_dup(STDIN_FILENO);

  p = fork();

  int r;
  if (p < 0)
    return -1;
  else if (p > 0)
  {
    //Run first command
    my_dup2(pipefd[1], STDOUT_FILENO); // modify stdout
    my_close(pipefd[0]);
    r = execute(s1, f1); //execute
    my_dup2(stdout_fd, 1);   // restore stdout
    my_close(pipefd[1]);
  }
  else
  {
    //Run second command
    my_dup2(pipefd[0], STDIN_FILENO); // modify stdin
    my_close(pipefd[1]);
    r = execute(s2, f2); //execute
    my_dup2(stdin_fd, 0); // restore stdin
    my_close(pipefd[0]);
    exit(r);
  }

  //close the fd's backup copy
  my_close(stdin_fd);
  my_close(stdout_fd);

  //wait for all child to end
  my_waitpid(-1, &status, 0);

  return status;
}

/* 
  execute 2 commands between |& 
  Return: 
    -1 -> something went wrong
    0 -> everything ok
*/
int execute_pipe_err(int s1, int f1, int s2, int f2)
{
  pid_t p;
  int pipefd[2];
  my_pipe(pipefd);
  int status;

  // save stdout and stdin
  int stdout_fd = my_dup(STDOUT_FILENO);
  int std_err = my_dup(STDERR_FILENO);
  int stdin_fd = my_dup(STDIN_FILENO);

  p = fork();

  int r;
  if (p < 0)
    return -1;  
  else if (p > 0)
  {
    //Run first command
    my_dup2(pipefd[1], STDOUT_FILENO); // modify stdout
    my_dup2(pipefd[1], STDERR_FILENO); // modify stderr
    my_close(pipefd[0]);

    r = execute(s1, f1);  // execute
    my_dup2(stdout_fd, STDOUT_FILENO); //restore stdout
    my_dup2(  std_err, STDERR_FILENO); //restore stderr
    my_close(pipefd[1]);
  }
  else
  {
    //Run second command
    my_dup2(pipefd[0], STDIN_FILENO); //modify stdin
    my_close(pipefd[1]);
    r = execute(s2, f2); //execute
    my_dup2(stdin_fd, 0); // restore stdin
    my_close(pipefd[0]);
    exit(r);
  }
  
  //close the fd's backup copy
  my_close(stdin_fd);
  my_close(stdout_fd);

  //wait for all child to end
  my_waitpid(-1, &status, 0);

  return status;
}