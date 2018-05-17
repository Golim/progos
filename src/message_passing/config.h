#ifndef CONFIG_h
#define CONFIG_h

#include "../util.h"

#define EXTREF "./"
#define EXTID (100)

extern int elaborate(msg *m);

int init_client();
int send_msg(msg *m);
int delete_client();

int init_server();
int start_listening();
int delete_server();
#endif