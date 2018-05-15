#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "../util.h"
#include "../message_passing/config.h"
#include "../logger/logger.h"

void genera_demone();
void esegui_programma();

int main(int argc, char const *argv[])
{
  if ( init_client() < 0 )
  {
    genera_demone();
    printf("[deamon generated]\n");
  }

  if(init_client() == 0 )
  {
    printf("[Pronto per eseguire il programma]\n");
    esegui_programma();
  }
  else
  {
    printf("[Errore, non è stata creata la coda]\n");
  }
  return 0;
}

void genera_demone()
{
  printf("[starting deamon]\n");

  pid_t fid = fork();
  if (fid < 0)
  {
    fprintf(stderr, "Errore nella crezione del figlio\n");
    exit(EXIT_FAILURE);
  }
  else if (fid == 0)
  {
    //Codice figlio:
    printf("[Figlio]:%d \n", getpid());
    if(init_server() < 0)
    {
      fprintf(stderr, "Errore nella crezione della coda\n");
      exit(EXIT_FAILURE);
    }
    int r = daemon(0, 1); //DA QUI IN POI é CODICE DEL DEMONE
    if (r < 0)
    {
      fprintf(stderr, "Errore nella crezione del demone\n");
      exit(EXIT_FAILURE);
    }
    printf("[Demone]:%d \n", getpid());
    start_listening(); // Quando finisce di ascoltare deve eliminare la coda e terminare con successo!!
    printf("[Elimino la coda!]\n");
    if(delete_server() <0)
    {
      fprintf(stderr, "Errore nel'eliminazione del server\n");
      exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
  }
  else{
  //Codice Padre:
  wait(&fid);
  }
}
void esegui_programma()
{
  char *msgtxt = malloc(sizeof(char) * 500);
  msg message;

  bool continua = TRUE;
  while (continua == TRUE)
  {
    printf("\n> ");
    long r = (long)fgets(msgtxt, MAXLEN, stdin);

    // terminate logger
    if (strstr(msgtxt, "exit") != NULL)
    {
      continua = FALSE;
    }
    if (strstr(msgtxt, "quit") != NULL)
    {
      message.type = TYPE_EXIT;
      continua = FALSE;
      send_msg(&message);
    }
    else
    {
      message.type = TYPE_TXT;
      *(strchr(msgtxt,'\n'))='\0';
      strcpy(message.msg_log.txt, msgtxt);
      send_msg(&message);
    }
  }
  free(msgtxt);
}