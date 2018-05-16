#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "program.h"

void genera_demone();
void esegui_programma();

int run(int format, char *filename, char *cmd)
{
  if (init_client() < 0)
  {
    genera_demone();
    printf("[deamon generated]\n");
  }

  if (init_client() == 0)
  {
    printf("[Pronto per eseguire il programma]\n");
    esegui_programma(format, filename, cmd);
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
    if (init_server() < 0)
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
    if (delete_server() < 0)
    {
      fprintf(stderr, "Errore nel'eliminazione del server\n");
      exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
  }
  else
  {
    //Codice Padre:
    wait(&fid);
  }
}
void esegui_programma(int format, char *filename, char *cmd)
{
  char *msgtxt = malloc(sizeof(char) * 500);

  stats(cmd, msgtxt, ",");

  msg message;
  strcpy(message.msg_log.txt, msgtxt);
  strcpy(message.msg_log.fn, filename);
  message.type = TYPE_CSV;
  send_msg(&message);
  free(msgtxt);
}
/*
void esegui_programma(int format, char *filename, char *cmd)
{
  //STOP THE DEAMON
  msg message;
  message.type = TYPE_EXIT;
  send_msg(&message);
}*/