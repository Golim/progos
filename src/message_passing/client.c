#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <errno.h>

#include "config.h"

int client_queue = -1;

key_t get_client_key()
{
  int i = ftok(EXTREF, EXTID);
  if (i < 0)
  {
    fprintf(stderr, "Errore generazione chiave!\n");
  }
  return i;
}

int init_client()
{
  client_queue = msgget(get_client_key(), 0666);
  if (client_queue < 0)
  {
    return (-1);
  }
  return 0;
}

int send_msg(msg *m)
{
  if (m->type < 0)
  {
    printf("[WARN]: message type less than zero \n");
  }
  if (msgsnd(client_queue, m, sizeof(struct msg) - sizeof(long), 0) < 0)
  {
    // EINVAL Invalid  msqid  value,  or  nonpositive  mtype value, or invalid msgsz value (less than 0 or greater than the system  value  MSG‐ MAX).
    fprintf(stderr, "Errore in invio [%d] %s\n", errno, strerror(errno));
  }
  return 0;
}
int delete_client()
{
  close(client_queue);
}
