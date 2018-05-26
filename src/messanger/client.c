#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#include "./msg_passing.h"
#include "../sys_wrapper/sys_wrapper.h"

int client_queue = -1;

key_t get_client_key()
{
  int i = ftok(EXTREF, EXTID);
  if (i < 0)
  {
    fprintf(stderr, "Error in generating key:[%d]\n", i);
  }
  return i;
}

int init_client()
{
  client_queue = msgget(get_client_key(), 0666);
  if (client_queue < 0)
  {
    return FAIL_STATUS;
  }
  return OK_STATUS;
}

int send_msg(msg *m)
{
  if (m->type < 0)
  {
    cond_print("[WARN]: message type less than zero \n");
  }
  if (msgsnd(client_queue, m, sizeof(struct msg) - sizeof(long), 0) < 0)
  {
    fprintf(stderr, "Error in sending message: [%d]\n", errno);
  }
  return 0;
}

int delete_client()
{
  return close(client_queue);
}
