#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

#include "program.h"

void genera_demone();
extern int esegui_programma(char *);
int run(char *cmd)
{
  if (init_client() < 0)
  {
    cond_print("[Daemon isn't running]\n");
    genera_demone();
  }

  if (init_client() == 0)
  {
    cond_print("[Ready for execution]\n");
    esegui_programma(cmd);
  }
  else
  {
    //TODO: gestisci con codice di ritorno
    fprintf(stderr, "Error: daemon still not running\n");
    return EXIT_FAILURE;
  }
  return 0;
}

void genera_demone()
{
  cond_print("[Generating Daemon]\n");

  pid_t fid = fork();
  if (fid < 0)
  {
    fprintf(stderr, "Error in creating daemon proces\n");
    exit(EXIT_FAILURE);
  }
  else if (fid == 0)
  {
    if (init_server() < 0)
    {
      exit(EXIT_FAILURE);
    }
    int r = daemon(1 - DAEMON_CH_DIR, 1 - DAEMON_CLOSE_OUT); //DA QUI IN POI é CODICE DEL DEMONE
    if (r < 0)
    {
      fprintf(stderr, "Error in creating daemon proces: [%d]\n", r);
      exit(EXIT_FAILURE);
    }
    printf("[Daemon is running. Pid:%d ] \n", getpid());
    start_listening(); // Quando finisce di ascoltare deve eliminare la coda e terminare con successo!!

    if (delete_server() < 0)
    {
      fprintf(stderr, "Error in terminating daemon\n");
      exit(EXIT_FAILURE);
    }
    cond_print("[Deamon killed, queue deleted]\n");
    exit(EXIT_SUCCESS);
  }
  else
  {
    wait(&fid);
  }
}
void esegui_e_logga(char *cmd)
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
      stats(cmd, msgtxt, " ,", mu, TRUE);
    else if (format == TYPE_TXT)
      stats(cmd, msgtxt, "\t", mu, TRUE);
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
    cond_print("[Success in sending]\n");
  }
  free(msgtxt);
}