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
#include <math.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

#define VERSION		"V1.00.00"
#define LEN_NAME_CLT	6
#define LEN_NAME_SRV	6
#define LEN_HOST_SRV    16

char *MSGERR = "\033[22;31mM-ERR\033[0m";
char *MSGOUT = "\033[39;49mM-OUT\033[0m";
char *MSGRCV = "\033[22;35mM-RCV\033[0m";
char *MSGSND = "\033[22;33mM-SND\033[0m";
char *MSGCON = "\033[22;32mM-CON\033[0m";
char *MSGDIS = "\033[22;34mM-DIS\033[0m";

int MAX_SD = 32768, LEN_SD = 6,
    MAX_LEN_MSG = 10000, LEN_MSG = 6,
    MAX_BUFFER_SOCK = 10000,
    NUM_SERVERS, NUM_CLIENTS;

struct clientStruct
{
  int  sd;
  char name[LEN_NAME_CLT];
};

struct serverStruct
{
  int  sd;
  int  port;
  int  max_num_sd;
  int  type;
  int  cant_clients;
  int  id;
  char name[LEN_NAME_SRV];
  char host[LEN_HOST_SRV];

  int  srv_port;
  char srv_name[LEN_NAME_CLT];

  struct clientStruct *clients;
  struct sockaddr_in address;
  pthread_t thread_server;

}*Servers;

struct messageGPS
{
  char system_code[5];
  int  message_type;
  int  unit_id;
};

int  loadConfig();
int  openServer(struct serverStruct *);
int  readSock(int, struct serverStruct *);
void *threadServer(void *);
char *msgStr(int);

int  processMsg(char *);
int  processMsgGps(int, int, char *);
unsigned long charHextoDec(char *, int);

int  printMsgNor(char *, int, int, char *, ...);
int  printMsgHex(char *, int, int, int, char *, char *, ...);

void printXml(xmlNode *);
void printXml2(xmlNode *);
void __nullGenericErrorFunc(void *, char *, ...);
char *getTagXml(xmlNode *, char *);

#endif /*MAIN_H*/
