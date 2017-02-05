#if 0

    BITACORA DE CAMBIOS 'main.c'
    
    JAS - Jaime Antonio Sepulveda
                 
    YYYY.MM.DD ATH - Notes
    2015.XX.XX JAS - V1.00.00 .

#endif

#include "main.h"

char *getTagXml(xmlNode *node_r, char *tag)
{
  xmlNode *cur = node_r->xmlChildrenNode;

  // get the id
  while(cur != NULL)
  {
    if(!xmlStrcmp(cur->name, (const xmlChar *)"attribute"))
    {
      printf("[%s]\n", (char*)xmlNodeGetContent(cur));

      xmlAttrPtr attr = cur->properties;

      if(!xmlStrcmp(attr->children->content, (const xmlChar*)"id"))
      {
        printf("[%s]\n", (char*)cur->children->content);
        printf("[%s]\n", (char*)attr->name);
        //return (char*)cur->content;
        return(NULL);
      }
    }

    cur = cur->next;
  }

  /*xmlNode *node = NULL;

  for(node = node_r; node; node = node->next)
  {
    if(node->type == XML_ELEMENT_NODE)
    {
      if(!strcmp((char *)node->name, tag))
      {
        fprintf(stdout, "tag = <%s>\n", node->name);
        //return(NULL);
      }
  
      //printXml(node->children);
      //fprintf(stdout, "</%s>", node->name);
    }//else
      //fprintf(stdout, "%s", node->content);
  }*/
  


  return("lol");
}

void printXml(xmlNode *a_node)
{
  xmlNode *node = NULL;

  for(node = a_node; node; node = node->next)
  {
    if(node->type == XML_ELEMENT_NODE)
    {
      fprintf(stdout, "<%s>", node->name);
      printXml(node->children);
      fprintf(stdout, "</%s>", node->name);
    }else
      fprintf(stdout, "%s", node->content);
  }

  return;
}

void __nullGenericErrorFunc(void* ctxt, char* msg, ...)
{
   return;
}

int main(int argc, char *argv[])
{
  /*xmlDoc  *document;
  xmlNode *root;

  if(argc < 2)
  {
    fprintf(stderr, "Usage: %s filename.xml\n", argv[0]);
    return 1;
  }

  xmlGenericErrorFunc handler = (xmlGenericErrorFunc)__nullGenericErrorFunc;
  initGenericErrorDefaultFunc(&handler);

  if((document = xmlReadFile(argv[1], NULL, 0)) == NULL)
  {
    printf("error: could not parse file %s\n", argv[1]);
    return(-1);
  }

  if((root = xmlDocGetRootElement(document)) == NULL)
  {
    printf("error: could not parse file %s\n", argv[1]);
    return(-2);
  }

  printXml(root);
  fprintf(stdout, "\n");
  getTagXml(root, "bb");
  xmlFreeDoc(document);
  xmlCleanupParser();*/

  int ix;

  printMsgNor(MSGOUT, -1, -1, "Monitor version:%s :)", VERSION);

  /*char s2[5];
  s2[0] = 0x00;
  s2[1] = 0xE5;
  s2[2] = 0x4A;
  s2[3] = 0xFF;
  s2[4] = '\0';
  charHextoDec(s2, 0);*/

  if(loadConfig() < 0)
    return(-1);

  for(ix = 0; ix < NUM_SERVERS; ix++)
  {
    if(openServer(&Servers[ix]) < 0)
      return(-1);
  }

  sigset_t myset;
  sigemptyset(&myset);

  while(1)
    sigsuspend(&myset);

  return(0);
}

int loadConfig()
{
  int ix, iy;

  //MaxSD = 0; /*/proc/sys/kernel/pid_max*/
  //LenSD = floor(log10(abs(MaxSD))) + 1;

  NUM_SERVERS = 2;
  NUM_CLIENTS = 10;

  Servers = (struct serverStruct*)malloc(NUM_SERVERS*sizeof(struct serverStruct));

  for(ix = 0; ix < NUM_SERVERS; ix++)
  {
    Servers[ix].clients = (struct clientStruct*)malloc(NUM_CLIENTS*sizeof(struct clientStruct));
   
    memset(Servers[ix].srv_name, 0, sizeof(Servers[ix].srv_name));

    for(iy = 0; iy < NUM_CLIENTS; iy++)
    {
      Servers[ix].clients[iy].sd = -1;
      memset(Servers[ix].clients[iy].name, 0, sizeof(Servers[ix].clients[iy].name));
    }
  }

  MAX_SD      = 32768;
  LEN_SD      = snprintf(NULL, 0, "%d", MAX_SD)+1;
  MAX_LEN_MSG = 10000;
  LEN_MSG     = snprintf(NULL, 0, "%d", MAX_LEN_MSG)+1;

  MAX_BUFFER_SOCK = 10000;

  Servers[0].port = 50000;
  Servers[0].type = 0;
  Servers[0].srv_port = 5100;
  snprintf(Servers[0].host, LEN_HOST_SRV, "%s", "127.0.0.1");
  snprintf(Servers[0].srv_name, LEN_NAME_CLT, "%s", "svbdd");
  snprintf(Servers[0].name, LEN_NAME_SRV, "%s", "srv11");
  Servers[0].max_num_sd = 3;
  Servers[0].cant_clients = 0;
  Servers[0].id = 0;

  Servers[1].port = 5100;
  Servers[1].type = 1;
  snprintf(Servers[1].host, LEN_HOST_SRV, "%s", "127.0.0.1");
  snprintf(Servers[1].name, LEN_NAME_SRV, "%s", "srv22");
  Servers[1].max_num_sd = 3;
  Servers[1].cant_clients = 0;
  Servers[1].id = 1;

  return(0);
}

int openServer(struct serverStruct *server)
{
  int option = 1;

  if((server->sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    printMsgNor(MSGERR, server->sd, -1, "Error socket : %s", msgStr(errno));
    return(-1);
  }

  if(setsockopt(server->sd, SOL_SOCKET, SO_REUSEADDR, (char *)&option, 
     sizeof(option)) == -1)
  {
    printMsgNor(MSGERR, server->sd, -1, "Error setsockopt : %s", msgStr(errno));
    return(-2);
  }

  server->address.sin_family = AF_INET;
  server->address.sin_addr.s_addr = INADDR_ANY;
  server->address.sin_port = htons(server->port);

  if(bind(server->sd, (struct sockaddr *)&server->address, sizeof(server->address)) == -1)
  {
    printMsgNor(MSGERR, server->sd, -1, "Error bind : %s", msgStr(errno));
    return(-3);
  }

  if(listen(server->sd, server->max_num_sd) == -1)
  {
    printMsgNor(MSGERR, server->sd, -1, "Error listen : %s", msgStr(errno));
    return(-4);
  }

  if(pthread_create(&server->thread_server, NULL, threadServer, (void *)server) != 0)
  {
    printMsgNor(MSGERR, server->sd, -1, "Error pthread_create : %s", msgStr(errno));
    return(-5);
  }

  return(0);
}

void *threadServer(void *parameter)
{
  int ix, maxsock, new;
  struct serverStruct *server = (struct serverStruct *)parameter;
  size_t size;
  fd_set active_fd_set, read_fd_set;

  printMsgNor(MSGOUT, server->sd, -1, "Server|%s|%d", server->name, server->port);

  FD_ZERO(&active_fd_set);
  FD_SET(server->sd, &active_fd_set);

  maxsock = server->sd;

  while(1)
  {
    read_fd_set = active_fd_set;
    select(maxsock+1, &read_fd_set, NULL, NULL, NULL);

    for(ix = 0; ix <= maxsock; ix++)
    {
      if(FD_ISSET (ix, &read_fd_set))
      {
        if(ix == server->sd)
        {
          size = sizeof(server->address);
          new = accept(server->sd, (struct sockaddr *)&server->address, (socklen_t*)&size);

          printMsgNor(MSGCON, server->sd, new, "New|%d|%d", server->port, new);

          FD_SET(new, &active_fd_set);

          if(new > maxsock)
          {
            maxsock = new;
          }

        }else
        {
          if(readSock(ix, server) < 0)
          {
            close(ix);
            FD_CLR(ix, &active_fd_set);
          }
        }
      }
    }
  }

  return(NULL);
}

int readSock(int client_sd, struct serverStruct *server)
{
  char buffer[MAX_BUFFER_SOCK];
  int  nbytes;

  memset(buffer, 0, sizeof(buffer));
  nbytes = read(client_sd, buffer, MAX_BUFFER_SOCK);

  if(nbytes == -1)
  {
    printMsgNor(MSGERR, server->sd, client_sd, "Error read : %s", msgStr(errno));
    return(-1);
  }
  else if(nbytes == 0)
  {
    printMsgNor(MSGDIS, server->sd, client_sd, "Closed|%d|%d", server->port, client_sd);
    return(-2);
  }
  else
  {
    if(server->type == 0)
    {
      printMsgHex(MSGRCV, server->sd, client_sd, nbytes, buffer, "%s", "NULL");
      processMsgGps(1, nbytes, buffer);
    }else
    {
      printMsgNor(MSGRCV, server->sd, client_sd, "%s", buffer);
      processMsg(buffer);
    }
    return(0);
  }
}

char *msgStr(int err_code)
{
  char *str;

  /*Cosas*/

  str = strerror(err_code);

  return(str);
}

int processMsg(char *msg)
{

  return(0);
}

int processMsgGps(int type_msg, int len_msg, char *msg)
{
  int len_begin_msg = 9, ix;

  if(len_begin_msg > len_msg)
    return(-1);

  for(ix = 0; ix < len_begin_msg; ix++)
  {
    if(ix < 4)
    {
      fprintf(stdout, "%s%02X%s", ix == 0 ? "System Code [":"", msg[ix], ix+1 != 4 ? ":":"]\n");
    }else if(ix == 4)
    {
      fprintf(stdout, "%s%02X%s", ix == 4 ? "Message Type [":"", msg[ix], ix+1 == 4 ? ":":"]\n");
    }else
    {
      fprintf(stdout, "%s%02X%s", ix == 5 ? "Unit ID [":"", msg[ix], ix+1 != len_begin_msg ? ":":"]\n");
    }
  }

  fprintf(stdout, "Message Type [%lu]\n", charHextoDec(msg+4, 1));
  fprintf(stdout, "Unit ID [%lu]\n", charHextoDec(msg+5, 4));

  if(type_msg == 1)
    return(0);

  return(0);
}

unsigned long charHextoDec(char *msg, int len_msg)
{
  int ix;
  double exp = -1;
  unsigned long decimal = 0;
  unsigned char lower;
  unsigned char higher;

  for(ix = len_msg-1; ix >= 0; ix--)
  {
    lower = msg[ix] & 0x0F;
    higher = (msg[ix] >> 4) & 0x0F;

    decimal += lower*pow(16, ++exp);
    decimal += higher*pow(16, ++exp);

    //printf("%s%X %X%s", ix == len_msg-1 ? "[":"", lower, higher, ix != 0 ? " ":"]\n");
  }

  return(decimal);
}

int printMsgNor(char *type_msg, int sd_srv, int sd_client, char *fmt_msg, ...)
{
  int len_msg;
  char msg[MAX_LEN_MSG], csd_srv[LEN_SD], csd_client[LEN_SD];
  time_t timer;
  struct tm *tms;
  va_list argptr;

  va_start(argptr, fmt_msg);
  vsnprintf(msg, MAX_LEN_MSG, fmt_msg, argptr);
  len_msg = strlen(msg);
  va_end(argptr);

  timer = time(NULL);
  tms = localtime(&timer);

  snprintf(csd_srv, LEN_SD, "%d", sd_srv);
  snprintf(csd_client, LEN_SD, "%d", sd_client);

  fprintf(stdout, "%02d:%02d:%02d %s [%*s|%*s] %0*d [%s]\n", tms->tm_hour, tms->tm_min, tms->tm_sec,
          type_msg, LEN_SD-1, sd_srv == -1 ? "":csd_srv, LEN_SD-1, sd_client == -1 ? "":csd_client,
          LEN_MSG-1, len_msg, msg);

  return(0);
}

int printMsgHex(char *type_msg, int sd_srv, int sd_client, int len_msg, char *msg_hex, char *fmt_msg, ...)
{
  int ix;
  char msg[MAX_LEN_MSG], csd_srv[LEN_SD], csd_client[LEN_SD];
  time_t timer;
  struct tm *tms;
  va_list argptr;

  va_start(argptr, fmt_msg);
  vsnprintf(msg, MAX_LEN_MSG, fmt_msg, argptr);
  va_end(argptr);

  timer = time(NULL);
  tms = localtime(&timer);

  snprintf(csd_srv, LEN_SD, "%d", sd_srv);
  snprintf(csd_client, LEN_SD, "%d", sd_client);

  fprintf(stdout, "%02d:%02d:%02d %s [%*s|%*s] %0*d [", tms->tm_hour, tms->tm_min, tms->tm_sec,
          type_msg, LEN_SD-1, sd_srv == -1 ? "":csd_srv, LEN_SD-1, sd_client == -1 ? "":csd_client,
          LEN_MSG-1, len_msg);

  for(ix = 0; ix < len_msg; ix++)
    fprintf(stdout, "%02X%s", (unsigned char)msg_hex[ix], ix+1 != len_msg ? ":":"]\n");
    //fprintf(stdout, "%02hhX%s", msg_hex[ix], ix+1 != len_msg ? ":":"]\n");

  return(0);
}

















