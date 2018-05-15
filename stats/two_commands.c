#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

typedef char* string;

int execute_two_commands_piped(char* cmd1, char *cmd2);
int execute_two_commands_endConnected(char* cmd1, char *cmd2);
int execute_two_commands_orConnected(char* cmd1, char *cmd2);

int main(int argc,char **argv)
{

    system("err.txt > wc");

    if(argc < 3)
    {
        perror("insufficient args!\nUsage: ./a.out <\"command before pipe\"> <\"command after pipe\">");
        exit(EXIT_FAILURE);
    }

    printf("arg1: %s\narg2: %s\n", argv[1], argv[2]);

    //execute_two_commands_piped(argv[1], argv[2]);
    execute_two_commands_endConnected(argv[1], argv[2]);

    return 0;
}

/*
execute 2 commands between pipe
return: 
    -1 -> something went wrong
    0 -> everything ok
*/
int execute_two_commands_piped(char* cmd1, char *cmd2)
{
    /*====== execute first command =======*/
    //1. create file descriptor
    int first_out = open("/tmp/first_out.txt", O_WRONLY | O_CREAT, 0666);
    if(first_out < 0)
        return -1;
    //2. save stdout fd, and close it
    int stdout_fd = dup(1);
    close(1);
    //3. reassign output
    dup2(first_out, STDOUT_FILENO);
    //4. execute
    system(cmd1);
    
    /*====== execute second command =======*/
    //1. redirect the output file to stdin
    strcat(cmd2, " < /tmp/first_out.txt");
    //2. restore stdout
    close(first_out);
    dup2(stdout_fd, 1);
    //3. execute
    system(cmd2);

    //remove temp file
    system("rm /tmp/first_out.txt");

    return 0;
}

/*
execute 2 commands between and
return: 
    -1 -> something went wrong
    0 -> everything ok
*/
int execute_two_commands_endConnected(char* cmd1, char *cmd2)
{
    if(system(cmd1) == 0)
        system(cmd2);
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
int execute_two_commands_orConnected(char* cmd1, char *cmd2)
{
    // lazy evaluation 
    if(system(cmd1) == 0)
        return 0;

    if(system(cmd2) == 0)    
        return 0;

    return 2;
}