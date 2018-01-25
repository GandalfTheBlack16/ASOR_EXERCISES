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
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {
  /* code */
  struct addrinfo hints, *res;
  struct sockaddr cli;
  char buff[81], host[NI_MAXHOST], serv[NI_MAXSERV];

  hints.ai_flags = AI_PASSIVE;
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_protocol = 0;

  //ARGS: Host ip, host port, hints struct, results struct
  getaddrinfo (argv[1], argv[2], &hints, &res);
  //CREATE SOCKET
  //Args: IP Family, SOCKET protocol, flags
  int sock = socket (res->ai_family, res->ai_socktype, 0);
  //Bind ai_addr to socket
  bind (sock, (struct sockaddr *)res->ai_addr, res->ai_addrlen);
  freeaddrinfo (res);
  //Main loop
  while (1){
    socklen_t clilen = sizeof(cli);
    memset (host, '\0', NI_MAXHOST-1);
    //Recive Data from Client
    //Args:  Socket, buffer for data storage, buffer length, source client addr, client addr lenght
    int bytes = recvfrom (sock, buff, 80, 0, (struct sockaddr *) &cli, &clilen);

    if (bytes > 0){
      getnameinfo ((struct sockaddr *) &cli, clilen, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST);
      host[NI_MAXHOST-1]='\n';
      sendto (sock, host, NI_MAXHOST, 0, (struct sockaddr *) &cli, clilen);
    }
  }
  return 0;
}
