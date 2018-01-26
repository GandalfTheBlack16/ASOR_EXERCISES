/*
 *
 *
 * Copyright 2018 GandalfTheBlack <gandalf@debian>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <string.h>

#define MAX_BUFF 128

int main(int argc, char const *argv[]) {
  /* code */
  pid_t child = fork();
  if (child < 0){
    perror ("fork()");
    exit (1);
  }
  //Read process
  if (child == 0){

    char buff[MAX_BUFF];
    int bytes;
    int fifo1 = open(argv[1], O_RDONLY);
    if (fifo1 < 0){
      fprintf(stderr, "ERROR: Cannot create pipes\n");
      exit (1);
    }
    printf("Conexion de recepcion establecida!\n");

    bytes = read (fifo1, &buff, sizeof(buff));
    while (bytes != 0){

      if (bytes > 0){
        buff[bytes] = '\0';
        printf("Data read from %s: %s\n", argv[1], buff);
      }
      bytes = read(fifo1, &buff, sizeof(buff));
    }
    if (close (fifo1) < 0){
      perror ("close()");
      exit(1);
    }
    printf ("Fifo %s closed sucessfully", argv[1]);

  }
  //Write process
  else{

    char buff1[MAX_BUFF], tmp[MAX_BUFF];
    char *scan;
    int fifo2 = open (argv[2], O_WRONLY);
    if (fifo2 < 0){
      fprintf(stderr, "ERROR: Cannot create pipes\n");
      exit (1);
    }
    printf("Conexion de envío establecida!\n");

    scan = fgets (tmp, sizeof(tmp), stdin);
    while (scan != NULL) {
      strcpy(buff1, tmp);
      write (fifo2, &buff1, sizeof(buff1));
      scan = fgets (tmp, sizeof(tmp), stdin);

    }
    if (close (fifo2) < 0){
      perror ("close()");
      exit(1);
    }
    printf ("Fifo %s closed sucessfully", argv[2]);
  }

  return 0;
}
