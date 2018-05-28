#ifndef SYS_WRAPPER_h
#define SYS_WRAPPER_h

//Wrapper for system call exit, it prints to stderr the error number and an explanation string.
void my_exit(int s);

// Wrapper for system call wait
// int wait (int * status)
int my_wait(int *status);

// Wrapper for system call waitpid
// pid_t waitpid(pid_t pid, int *wstatus, int options)
pid_t my_waitpid(pid_t pid, int *wstatus, int options);

// Wrapper for system call dup
// int dup(int oldfd)
int my_dup(int oldfd);

// Wrapper for system call dup2
// int dup(int oldfd, int newfd)
int my_dup2(int oldfd, int newfd);

// Wrapper for system call pipe
// int pipe(int pipefd[2])
int my_pipe(int pipefd[2]);

// Wrapper for system call close
// int close(int fd)
int my_close(int fd);

// Wrapper for system call remove
//int remove(const char *pathname)
int my_remove(const char *pathname);

// Wrapper for system call close
// int open(const char *pathname, int flags, mode_t mode)
int my_open(const char *pathname, int flags, mode_t mode);

// Function that return the strig given an error number
char *my_strerr(int s);

#endif