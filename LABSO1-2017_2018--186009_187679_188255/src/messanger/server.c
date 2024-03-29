#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>

#include "./msg_passing.h"
#include "../sys_wrapper/sys_wrapper.h"

int server_queue;

key_t get_server_key()
{
  int fd = open(EXTREF, 0777, O_CREAT);
  if(fd > 0 )
    my_close(fd);
  int i = ftok(EXTREF, EXTID);
  if (i < 0)
  {
    fprintf(stderr, "Errore in generating key:[%d]\n", i);
  }
  return i;
}

/**
 * Initialize the server side of queue
 * Return OK_STATUS if it was correctly inizialized
 *        FAIL_STATUS otherwise
 * */
int init_server()
{
  //server_queue = msgget(get_server_key(), IPC_CREAT | IPC_EXCL | 0777);
  server_queue = msgget(get_server_key(), IPC_CREAT | 0777);
  if (server_queue < 0)
  {
    fprintf(stderr, "Error in creating message queue: [%d] \n", errno);
    return FAIL_STATUS;
  }
  return OK_STATUS;
}

/**
 * Keep receiving messages until STOP_STATUS is detected
 * */
int start_listening()
{
  struct msg *m = malloc(sizeof(struct msg));
  bool continua = TRUE;
  while (continua == TRUE)
  {
    if (msgrcv(server_queue, m, sizeof(struct msg) - sizeof(long), 0, 0) < 0)
    {
      fprintf(stderr, "Error in reciving message [%d]\n", errno);
    }
    else
    {
      if (elaborate(m) == STOP_STATUS)
        continua = FALSE;
    }
  }
  free(m);
  return OK_STATUS;
}

/**
 * delete the server's access queue
 * */
int delete_server()
{
  return msgctl(server_queue, IPC_RMID, NULL);
}