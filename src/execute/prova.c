#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
  int std_err = dup(STDERR_FILENO);
  
  dup2(STDOUT_FILENO, STDERR_FILENO);
  fprintf(stderr, "[TO OUT]\n");

  dup2(std_err ,STDERR_FILENO);
  fprintf(stderr, "[TO ERR]\n");
  return 0;
}
