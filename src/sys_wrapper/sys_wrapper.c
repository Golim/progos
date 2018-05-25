#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../util/util.h"

char * my_strerr(int s);

//Wrapper for system call exit, it prints to stderr the error number and an explanation string.
void my_exit(int s)
{
  fprintf(stderr, "Error during the execution: [%d]\n\t%s\n", s, my_strerr(s));
  exit(s); 
}

// Wrapper for system call wait
// int wait (int * status)
int my_wait(int * status)
{
  int r = wait(status);
  if(r == -1)
    my_exit(-1); //Set the appropriate return code that would be recognized by my_strerr
  else
    return r;
}

// Wrapper for system call close
// pid_t waitpid(pid_t pid, int *wstatus, int options)
pid_t my_waitpid(pid_t pid, int *wstatus, int options)
{
  int r = waitpid(pid, wstatus, options);
  if(r == -1)
    my_exit(-1);
  else
    return r;
}

// Wrapper for system call dup
// int dup(int oldfd)
int my_dup(int oldfd)
{
  int r = dup(oldfd);
  if(r == -1)
    my_exit(-1);
  else
    return r;
}

// Wrapper for system call dup2
// int dup(int oldfd, int newfd)
int my_dup2(int oldfd, int newfd)
{
  int r = dup2(oldfd, newfd);
  if(r == -1)
    my_exit(-1);
  else
    return r;
}

// Wrapper for system call pipe
// int pipe(int pipefd[2])
int my_pipe(int pipefd[2])
{
  int r = pipe(pipefd);
  if(r == -1)
    my_exit(-1);
  else
    return r;
}

// Wrapper for system call close
// int close(int fd)
int my_close(int fd)
{
  int r = close(fd);
  if(r == -1)
    my_exit(-1);
  else
    return r;
}

// Wraapper for system call remove
//int remove(const char *pathname)
int my_remove(const char *pathname)
{
  int r = remove(pathname);
  if(r == -1)
    my_exit(-1);
  else
    return r;
}

// Wrapper for system call close
// int open(const char *pathname, int flags, mode_t mode)
int my_open(const char *pathname, int flags, mode_t mode)
{
  int r = open(pathname, flags, mode);
  if(r == -1)
    my_exit(-1);
  else
    return r;
}

// Function that return the strig given an error number
char * my_strerr(int s)
{
  switch(s)
  {
    case -1: // Keep -1 for generic error, use another number for more specific
      return "generic error";
    break;
    case ARG_TOO_FEW : 
      return "More arguments expeted";
    break;
    default:
      return "";
  }
  return "";
}