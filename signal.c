/*This program capture SIGINT and SIGTSTP signal into a loop */
/*Finally print number of catched sigs of each type*/
#include <stdio.h>
#include <signal.h>

int cont_SIGINT = 0;
int cont_SIGTSTP = 0;

void handler (int signal){
  //Signal = SIGINT
  if (signal == 2)
    cont_SIGINT++;
  //Signal = SIGTSTP
  if (signal == 18 || signal == 20 || signal == 24)
    cont_SIGTSTP++;
}

int main (int argc, char ** argv)
{
  struct sigaction act;
  sigset_t set;

  act.sa_handler = handler;
  act.sa_flags = 0;

  sigaction (SIGINT, &act, NULL);
  sigaction (SIGTSTP, &act, NULL);

  sigemptyset (&set);
  sigdelset (&set, SIGINT);
  sigdelset (&set, SIGTSTP);
  sigprocmask (SIG_SETMASK, &set, NULL);

  while (cont_SIGINT + cont_SIGTSTP < 10 ){
    printf("# SIGINT: %d\n # SIGTSTP: %d\n", cont_SIGINT, cont_SIGTSTP);
    sigsuspend (&set);
  }

  printf("# SIGINT: %d\n # SIGTSTP: %d\n", cont_SIGINT, cont_SIGTSTP);

  return 0;
}
