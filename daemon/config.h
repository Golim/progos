// Unique references for queue
#define EXTREF "./"
#define EXTID  (12)

#define TMPLEN (100)

#include <stdio.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

char *tmpfiletpl="queue.tmp";

int msgkey(int mode) { // mode == 0 for READ, 1 for write
	char tmpfile[TMPLEN];
	int ft;
	char cwd[1024];
	char *cnt="---";
	
	getcwd(cwd, sizeof(cwd));
	
	strcpy(tmpfile, EXTREF);
	strcat(tmpfile, tmpfiletpl);
	ft = open(tmpfile, O_WRONLY | O_CREAT, 0777);
	
	write(ft, cnt, strlen(cnt)+1);
	
	close(ft);
	
	remove(tmpfile);
	
	// ftok to generate unique key
	int key = ftok(tmpfile, EXTID);
	if (mode==0) remove(tmpfile);
	//printf("extref='%s', tmpfile='%s' (ft=%d), extid='%d', msgkey='%d'\n", EXTREF, tmpfile, ft, EXTID, key);
	return (key);
}
