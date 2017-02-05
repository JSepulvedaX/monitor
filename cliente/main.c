#if 0

    BITACORA DE CAMBIOS 'main.c'
    
    JAS - Jaime Antonio Sepulveda
                 
    YYYY.MM.DD ATH - Notes
    2015.XX.XX JAS - V1.00.00 .

#endif

#include "main.h"

int main(int argc, char *argv[])
{
  char *buffer;
  int sd, n;

  if(argc != 3)
  {
    printf("Error en la cantidad de argumentos\n");
    return(-1);
  }

  Servers = (struct serverStruct*)malloc(1*sizeof(struct serverStruct));  

  snprintf(Servers[0].host, LEN_HOST_SRV+1, "%s", argv[1]);
  Servers[0].port = atoi(argv[2]);
  Servers[0].sd = -1;
  Servers[0].id = 0;

  openClient(&Servers[0]);

  while(1)
  {
    /*printf("Please enter the message: ");
    scanf("%s",buffer);

    buffer[strlen(buffer)] = '\0';*/

   buffer = (char *)malloc(70*sizeof(char));
   //memset(buffer, '\0', sizeof(buffer));

   buffer[0] =  0x4D; 
   buffer[1] =  0x43;
   buffer[2] =  0x47;
   buffer[3] =  0x50;
   buffer[4] =  0x00;
   buffer[5] =  0x06;
   buffer[6] =  0x00;
   buffer[7] =  0x00;
   buffer[8] =  0x00;
   buffer[9] =  0x08;
   buffer[10] = 0x1A;
   buffer[11] = 0x02;
   buffer[12] = 0x02;
   buffer[13] = 0x12;
   buffer[14] = 0x04;
   buffer[15] = 0x00;
   buffer[16] = 0x00;
   buffer[17] = 0x00;
   buffer[18] = 0x21;
   buffer[19] = 0x00;
   buffer[20] = 0x62;
   buffer[21] = 0x30;
   buffer[22] = 0x00;
   buffer[23] = 0x00;
   buffer[24] = 0x00;
   buffer[25] = 0x6B;
   buffer[26] = 0x00;
   buffer[26] = 0xE1;
   buffer[28] = 0x00;
   buffer[29] = 0x00;
   buffer[30] = 0x00;
   buffer[31] = 0x00;
   buffer[32] = 0x00;
   buffer[33] = 0x00;
   buffer[34] = 0x00;
   buffer[35] = 0x00;
   buffer[36] = 0x00;
   buffer[37] = 0x00;
   buffer[38] = 0xE5;
   buffer[39] = 0xA1;
   buffer[40] = 0x00;
   buffer[41] = 0x04;
   buffer[42] = 0x02;
   buffer[43] = 0x06;
   buffer[44] = 0x61;
   buffer[45] = 0x4E;
   buffer[46] = 0xA3;
   buffer[47] = 0x03;
   buffer[48] = 0x18;
   buffer[49] = 0x1A;
   buffer[50] = 0x57;
   buffer[51] = 0x03;
   buffer[52] = 0x4E;
   buffer[53] = 0x12;
   buffer[54] = 0x00;
   buffer[55] = 0x00;
   buffer[56] = 0x00;
   buffer[57] = 0x00;
   buffer[58] = 0x00;
   buffer[59] = 0x00;
   buffer[60] = 0x00;
   buffer[61] = 0x00;
   buffer[62] = 0x15;
   buffer[63] = 0x25;
   buffer[64] = 0x07;
   buffer[65] = 0x14;
   buffer[66] = 0x03;
   buffer[67] = 0xD6;
   buffer[68] = 0x07;
   buffer[69] = 0x49;
   buffer[70] = '\0';

    n = write(Servers[0].sd,buffer,70);
    if(n < 0) 
      printf("ERROR writing to socket\n");
    break;
  }

  close(sd);

  return(0);
}

int openClient(struct serverStruct *server)
{
  server->address.sin_family = AF_INET;
  server->address.sin_port = htons(server->port);

  if(inet_aton(server->host, &server->address.sin_addr) == 0)
  {
    /*error*/
    return(-1);
  }

  if((server->sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    /*error*/
    return(-2);
  }

  if(pthread_create(&server->thread_server, NULL, threadServer, &server->id) != 0)
  {
    /*error*/
    return(-3);
  }

  sleep(2);

  return(server->sd);
}

void *threadServer(void *parameter)
{
  int server_id = *((int*)parameter);
  fd_set active_fd_set, read_fd_set;

  FD_ZERO(&active_fd_set);
  FD_SET(Servers[server_id].sd, &active_fd_set);
  read_fd_set = active_fd_set;

  while(1)
  {
    if(connect(Servers[server_id].sd, (struct sockaddr *)&Servers[server_id].address, sizeof(Servers[server_id].address)) == -1)
    {
      printf("Fallo :(\n");
      sleep(2);
      continue;
    }else
      printf("Nos conectamos :)\n");

    select(Servers[server_id].sd+1, &read_fd_set, NULL, NULL, NULL);
 
    if(readSock(server_id) < 0)
    {
      close(Servers[server_id].sd);
      //FD_CLR(server_id, &active_fd_set);
    }

    if((Servers[server_id].sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
      return(NULL);
  }

  return(NULL);
}

int readSock(int server_id)
{
  char buffer[MAX_BUFFER_SOCK];
  int  nbytes;

  memset(buffer, 0, sizeof(buffer));
  nbytes = read(Servers[server_id].sd, buffer, MAX_BUFFER_SOCK);

  if(nbytes == -1)
  {
    //printMsg(MSGERR, Servers[server_id].sd, client_sd, "Error read : %s", msgStr(errno));
    return(-1);
  }
  else if(nbytes == 0)
  {
    //printMsg(MSGDIS, Servers[server_id].sd, client_sd, "Closed|%d|%d", Servers[server_id].port,
             //client_sd);
    return(-2);
  }
  else
  {
    //printMsg(MSGRCV, Servers[server_id].sd, client_sd, "%s", buffer);
    return(0);
  }
}


