#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <string.h>
#include <unistd.h>
#define MAXLEN (100)
 
// structure for message queue
struct mesg_buffer {
    long mesg_type;
    char mesg_text[MAXLEN];
	char file_name[MAXLEN];
} message;

int check_daemon_exist();
 
int main() {
    long r;
    key_t key;
	int msgid;
	char msgtxt[MAXLEN];
    
    if(check_daemon_exist() == 0){
		printf("Daemon is not running\nDaemon starting...");

		char *args[] = {NULL, NULL};
		char *cmd = malloc(MAXLEN);
		/*getcwd(cmd, MAXLEN);
		strcat(cmd, "/mydaemon");*/
		strcpy(cmd, "./mydaemon");
		system(cmd);
		printf(" Started!\n");
	}
		
	//create the queue and send messages
	key = msgkey(1);
	msgid = msgget(key, 0666);
	message.mesg_type = 1;
    
    while(1){
		
		printf("> ");
		r=(long)fgets(msgtxt, MAXLEN, stdin);
		
		if(strstr(msgtxt, "exit") != NULL){
			exit(0);
		}
		
		strcpy(message.mesg_text, msgtxt);
		msgsnd(msgid, &message, sizeof(message), 0);
		printf("\n");
		
		if(strstr(msgtxt, "quit") != NULL){
			exit(0);
		}
	}
    return 0;
}

int check_daemon_exist(){
	int size = 0;
	
	system("ps -xj | grep -v grep | grep mydaemon >> /tmp/.check");
	FILE *fp = fopen("/tmp/.check", "r");
	if (fp != NULL) {
		fseek (fp, 0, SEEK_END);
		size = ftell(fp);
	}
	fclose(fp);
	system("rm /tmp/.check");
	
	return size;
}
