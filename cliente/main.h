#if 0

    BITACORA DE CAMBIOS 'main.h'
    
    JAS - Jaime Antonio Sepulveda
                 
    YYYY.MM.DD ATH - Notes
    2015.XX.XX JAS - V1.00.00 .

#endif

#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define VERSION         "V1.00.00"
#define LEN_HOST_SRV    15

int  MAX_BUFFER_SOCK = 10000;
char *MessageHello = "<tx ty='hello'><name></name></tx>";

struct serverStruct
{
  int id;
  int sd;
  int port;
  char host[LEN_HOST_SRV];

  struct sockaddr_in address;
  pthread_t thread_server;

}*Servers;

int  openClient(struct serverStruct *);
int  readSock(int);
void *threadServer(void *);

#endif /*MAIN_H*/
