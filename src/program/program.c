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

int run(int format, char *filename, char *cmd, bool mu)
{
  if (init_client() < 0)
  {
    cond_print("[Demone non in ascolto]\n");
    genera_demone();
  }

  if (init_client() == 0)
  {
    cond_print("[Pronto per eseguire il programma]\n");
    esegui_programma(format, filename, cmd, mu);
  }
  else
  {
    //TODO: gestisci con codice di ritorno
    fprintf(stderr, "Errore, non è stata creata la coda\n");
  }
  return 0;
}

void genera_demone()
{
  cond_print("[Generazione Demone]\n");

  pid_t fid = fork();
  if (fid < 0)
  {
    fprintf(stderr, "Errore nella crezione del figlio\n");
    exit(EXIT_FAILURE);
  }
  else if (fid == 0)
  {
    if (init_server() < 0)
    {
      fprintf(stderr, "Errore nella crezione della coda\n");
      exit(EXIT_FAILURE);
    }
    int r = daemon(1 - DAEMON_CH_DIR, 1 - DAEMON_CLOSE_OUT); //DA QUI IN POI é CODICE DEL DEMONE
    if (r < 0)
    {
      fprintf(stderr, "Errore nella crezione del demone\n");
      exit(EXIT_FAILURE);
    }
    printf("[Demone lanciato: %d ] \n", getpid());
    start_listening(); // Quando finisce di ascoltare deve eliminare la coda e terminare con successo!!

    if (delete_server() < 0)
    {
      fprintf(stderr, "Errore nel'eliminazione del server\n");
      exit(EXIT_FAILURE);
    }
    cond_print("[Demone Terminato, eliminata coda!]\n");
    exit(EXIT_SUCCESS);
  }
  else
  {
    //Codice Padre:
    wait(&fid);
  }
}
void esegui_programma(int format, char *filename, char *cmd, bool mu)
{
  //TODO: gestire invio file con percorso percorso assoluto e corrente
  char *msgtxt = malloc(sizeof(char) * MAX_LEN_STAT);
  msg message;

  if (strcmp(cmd, "stop_daemon") == 0)
    message.type = TYPE_EXIT;
  else
  {
    message.type = format;
    cond_print("\n---\t output %s \t---\n", cmd);
    if (format == TYPE_CSV)
      stats(cmd, msgtxt, " ,", mu);
    else if (format == TYPE_TXT)
      stats(cmd, msgtxt, "\t", mu);
    cond_print("------------------------\n", cmd);
  }

  strcpy(message.msg_log.txt, msgtxt);
  strcpy(message.msg_log.fn, filename);

  int r = send_msg(&message);
  if (r < 0)
  {
    fprintf(stderr, "Errore nel'invio del log:[%d] \n", r);
  }
  else
  {
    cond_print("[Messaggio di log inviato correttamente]\n");
  }
  free(msgtxt);
}