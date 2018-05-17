#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

typedef char* string;

int execute_two_commands_piped(char* cmd1[], char *cmd2[]);
int execute_two_commands_endConnected(char* cmd1, char *cmd2);
int execute_two_commands_orConnected(char* cmd1, char *cmd2);

int main(int argc,char **argv)
{
    /*
    if(argc < 3)
    {
        perror("insufficient args!\nUsage: ./a.out <\"command before pipe\"> <\"command after pipe\">");
        exit(EXIT_FAILURE);
    }
*/
    char *argv1[] = {"date", NULL};
    char *argv2[] = {"wc", "-c", NULL};


    //printf("arg1: %s\narg2: %s\n\n", argv[1], argv[2]);

    execute_two_commands_piped(argv1, argv2);
    //execute_two_commands_endConnected(argv[1], argv[2]);
    //execute_two_commands_orConnected(argv[1], argv[2]);

    return 0;
}

/*
execute 2 commands between pipe
return: 
    -1 -> something went wrong
    0 -> everything ok
*/
int execute_two_commands_piped(char* cmd1[], char *cmd2[])
{
    //== create two process and make them comunicate using a pipe
    pid_t pid1, pid2; 
    int pipefd[2];  //bidirectional pipe
    pipe(pipefd);
    int status;

    // save stdout and stdin
    int stdout_fd = dup(STDOUT_FILENO);
    int stdin_fd = dup(STDIN_FILENO);

    //===== first process =====
    pid1 = fork();
    if(pid1 < 0)
        return -1;

    if(pid1 == 0)
    {
        // move stdout to the pipe
        dup2(pipefd[1], STDOUT_FILENO);
        
        //close other side of pipe
        close(pipefd[0]);

        //execute command 
        execvp(cmd1[0], cmd1);

        return -1;
    }
    //=========================

    //===== second process =====
    pid2 = fork();
    if(pid2 < 0)
        return -1;

    if(pid2 == 0)
    {
        //move stdin to the pipe
        dup2(pipefd[0], STDIN_FILENO);

        //close other side of pipe
        close(pipefd[1]);

        //execute command
        execvp(cmd2[0], cmd2);

        return -1;
    }
    //=========================

    //restore stdin and stdout
    close(pipefd[0]);
    close(pipefd[1]);
    dup2(stdin_fd, 0);
    dup2(stdout_fd, 1);
    close(stdin_fd);
    close(stdout_fd);

    //wait for all child to end
    waitpid(-1, &status, 0);


    /* ****** FILE BASED ******
    //====== execute first command =======
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
    
    //====== execute second command =======
    //1. redirect the output file to stdin
    strcat(cmd2, " < /tmp/first_out.txt");
    //2. restore stdout
    close(first_out);
    dup2(stdout_fd, 1);
    //3. execute
    system(cmd2);

    //remove temp file
    system("rm /tmp/first_out.txt");
    ********************** */

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