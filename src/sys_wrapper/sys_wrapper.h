void my_exit(int s);
int my_wait(int * status);
pid_t my_waitpid(pid_t pid, int *wstatus, int options);
int my_dup(int oldfd);
int my_dup2(int oldfd, int newfd);
int my_pipe(int pipefd[2]);
int my_close(int fd);
int my_remove(const char *pathname);
int my_open(const char *pathname, int flags, mode_t mode);
