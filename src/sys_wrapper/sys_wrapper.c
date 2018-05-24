#include <stdio.h>
#include <stdlib.h>

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
void my_wait(int * s)
{
  int r = wait(s);
  if(r == -1) // See Return Value in man
    my_exit(-1);
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