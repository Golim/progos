#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <errno.h>

#include "../util.h"
#include "./config.h"

int server_queue;

key_t get_server_key()
{
  int fd = open(EXTREF, 0777, O_CREAT);
  close(fd);
  remove(EXTREF);
  int i = ftok(EXTREF, EXTID);
  if (i < 0)
  {
    fprintf(stderr, "Errore generazione chiave!\n");
  }
  return i;
}
int init_server()
{
  server_queue = msgget(get_server_key(), IPC_CREAT | IPC_EXCL | 0777);
  if (server_queue < 0)
  {
    printf("Errore in creazione coda [%d] %s\n", errno, strerror(errno));
    return -1;
  }
  return 0;
}
int start_listening()
{
  struct msg *m = malloc(sizeof(struct msg));
  bool continua = TRUE;
  while (continua == TRUE)
  {
    if (msgrcv(server_queue, m, sizeof(struct msg) - sizeof(long), 0, 0) < 0)
    {
      // EINVAL Invalid  msqid  value,  or  nonpositive  mtype value, or invalid msgsz value (less than 0 or greater than the system  value  MSG‐ MAX).
      printf("Errore in lettura [%d] %s\n", errno, strerror(errno));
    }
    else
    {
      if (elaborate(m) == -1)
        continua = FALSE;
    }
  }
  free(m);
  return 0;
}

int delete_server()
{
  return msgctl(server_queue, IPC_RMID, NULL);
}