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


#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>

#define MAX_BUFF_LEN 64

int main(int argc, char **argv)
{

	struct addrinfo hints;
	struct addrinfo *res, *iterator;

	char buff[MAX_BUFF_LEN];

	hints.ai_family = 0;
	hints.ai_socktype = 0;

	if (getaddrinfo(argv[1], NULL, &hints, &res) < 0){
		perror("getaddrinfo():");
		exit(EXIT_FAILURE);
	}

	for (iterator = res; iterator != NULL; iterator = iterator->ai_next){

		if (getnameinfo(iterator->ai_addr, sizeof(struct sockaddr), buff, MAX_BUFF_LEN, NULL, NULL, NI_NUMERICHOST) < 0){
			perror("getnameinfo()");
		}
		else
			fprintf(stdout, "%s %d %d", buff, iterator->ai_family, iterator->ai_socktype);
	}

	return 0;
}
