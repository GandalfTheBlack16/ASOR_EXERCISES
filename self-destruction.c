/*This program sets a Self-Destruction Countdown. When timer expires exec file is removed*/
/*User can cancell self-destruction Countdown sending INTERRUPT Signal (Ctrl+C)*/

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <wait.h>

char *path;
pid_t pid;

void cancell (int signal){
  printf ("Self-Destruction sequence cancelled!\n");
  kill(pid, SIGTERM);
}
void destruct (int signal){
  printf ("Self-Destruction incoming...\n");
  execlp ("rm", "rm", path, NULL);
}

int main (int argc, char** argv){

  if (argc < 2){

    fprintf(stderr, "Time as 2nd Argument is needed\n");
    exit (EXIT_FAILURE);
  }

  struct sigaction alarm_act, cancell_act;
  sigset_t alarm_set, cancell_set;

  path = malloc (strlen(argv[0]));
  path = argv[0];

  sigemptyset (&alarm_set);
  sigdelset (&alarm_set, SIGALRM);
  sigprocmask (SIG_SETMASK, &alarm_set, NULL);

  sigemptyset (&cancell_set);
  sigdelset (&cancell_set, SIGINT);
  sigprocmask (SIG_SETMASK, &cancell_set, NULL);

  alarm_act.sa_handler = destruct;
  alarm_act.sa_flags = 0;

  cancell_act.sa_handler = cancell;
  cancell_act.sa_flags = 0;

  sigaction (SIGALRM, &alarm_act, NULL);
  sigaction (SIGINT, &cancell_act, NULL);

  //Start Self-Destruction Countdown
  alarm (atoi(argv[1]));
  printf ("Self-destruction at %d seconds\n", atoi(argv[1]));

  pid_t child = fork();
  if (child == 0){
    sigsuspend (&cancell_set);
    exit (1);
  }
  else if (child > 0){
    pid = child;
    sigsuspend (&alarm_set);
    exit(1);
  }
  free (path);
  return 0;
}
