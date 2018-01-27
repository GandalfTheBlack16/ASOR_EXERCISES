#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include <stdlib.h>
#include <wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#define _GNU_SOURCE


void handler (int signal){

  printf ("Executing Task...\n");
}

//Arg1: Time to timer expires
//Arg2: Programm to execute after alarm expires
int main(int argc, char const *argv[]) {
  /* code */
  pid_t child = fork();
  if (child == -1)
  {
    perror("Fork Error:");
    exit (EXIT_FAILURE);
  }
  //Father Process
  else if (child > 0){
    printf ("Ejecución programada en %s segundos por proceso %d\n", argv[1] ,child);
    wait(NULL);
    exit (EXIT_SUCCESS);
  }
  //Chld Process
  else{
    struct sigaction action;
    sigset_t blk_set;
    //Prepare Signal Set to block until process catch SIGALRM signal
    sigemptyset (&blk_set);
    sigdelset (&blk_set, SIGALRM);
    sigprocmask (SIG_SETMASK, &blk_set, NULL);
    //Launch signal timer
    alarm (atoi(argv[1]));
    //Modify SIGALRM handler
    action.sa_handler = handler;
    action.sa_flags = 0;
    sigaction (SIGALRM, &action, NULL);
    //Suspend Process Execution untill SIGALRM is catched
    sigsuspend (&blk_set);


    int fd[2];
    char buff[255];
    int pip = pipe (fd);
    if (pip < 0){
      perror ("Error Pipe");
      exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1){
      perror ("Error fork");
      exit (EXIT_FAILURE);
    }
    else if (pid == 0){

      dup2(fd[1],1);
      close(fd[1]);
      close (fd[0]);
      execlp (argv[2], argv[2], argv[3], NULL);
      perror ("ERROR execlp");
      exit (EXIT_FAILURE);
    }
    else{
      wait (NULL);

      int file = open ("/tmp/examen.out", O_CREAT | O_TRUNC | O_WRONLY, 0666);
      if (file < 0){
        perror ("Error Open");
        exit (EXIT_FAILURE);
      }

      int bytes = read (fd[0], &buff, sizeof(buff));
      write (file, &buff, bytes);

      //write (file, "\n", 1);
      close (file);
      close (fd[0]);
      exit(EXIT_SUCCESS);
    }

    exit (EXIT_SUCCESS);
  }

  return 0;
}
