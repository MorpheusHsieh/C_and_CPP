// Program: Echo Server
// Auther : Avatar
//Date    : 1998/02/09
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock.h>

#define MAXLINE 1024

void main(void)
{
   SOCKET   serv_sd, cli_sd;
   struct sockaddr_in serv, cli;
   int  cli_len, n;
   char str[MAXLINE];
   WSADATA wsadata;

   // Call WSAStartup() Register Winsock DLL login in

   if (WSAStartup(0x101, (LPWSADATA) &wsadata) != 0)
   {
      fprintf(stderr, "echo_srv: can'tuse Winsock DLL\n");
      exit(1);
   }

   // Open TCP socket

   if ( (serv_sd=socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
   {
      fprintf(stderr, "echo_srv: can't open TCPsocket\n");
      exit(1);
   }

   // Get IP address and port number of socket

   serv.sin_family = AF_INET;
   serv.sin_addr.s_addr = 0;
   serv.sin_port = htons(IPPORT_ECHO);

   // set IPPORT_ECHO is echo port

   if ( (bind(serv_sd, (LPSOCKADDR) &serv, sizeof(serv)) <0 ))
   {
      fprintf(stderr, "echo_srv: can't bind local address\n");
      exit(1);
   }

   // Call listen() let socket stand-by listen state, and most connect
   // simolation

   if ( listen(serv_sd, 5) < 0 )
   {
      fprintf(stderr, "echo_srv: listen() error\n");
      exit(1);
   }

   // Wait for clinet connect.
   // Note! Only One client connect

   cli_len=sizeof(cli);
   while (1)
   {
      printf("echo_srv: waiting for client\n");
      if ( (cli_sd=accept(serv_sd, (struct sockaddr *) &cli, &cli_len)) == SOCKET_ERROR )
      {
         fprintf(stderr, "echo_srv: accpet() error\n");
         closesocket(cli_sd);
      }
      else {
         while(1)
         {

            if ( (n=recv(cli_sd, str, MAXLINE, 0)) == 0)
            {
               fprintf(stderr, "echo_serv: connection closed\n");
               break;
            }
            else if (n == SOCKET_ERROR) {
               fprint(stderr, "echo_srv: recv() error!\n");
               break;
            }
            str[n]='\0';
            printf("echo_srv: %s", str);
            if ( send(cli_sd, str strlen(str), 0) == SOCKET_ERROR)
            {
               fprintf(stderr, "echo_srv: connection closed\n");
               break;
            }
         }
      }
   }

   // Winsock DLL is over
   closesocket(serv_sd);
   closesocket(cli_sd);
}

