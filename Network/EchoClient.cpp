// Program : Echo client
// Auther  : Morpheus
// Date    : 1998/02/09
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock.h>

#define MAXLINE 1024

void main()
{
   SOCKET  sd;
   struct sockaddr_in serv;
   char str[MAXLINE];
   int n;
   WSADATA wsadata;

   // Call WSAStartup() Register Winsock Dll

   if (WSAStartup(0x101, (LPWSADATA) &wsadata) != 0)
   {
      fprintf(stderr, "echo_srv: WSAStartup() fails!");
      exit(1);
   }
   printf("Echo serve IP: ");
   fgets(str, MAXLINE, stdin);

   // Write sockaddr_in struct (serv) include IP address, port number..etc

   serv.sin_family = AF_INET;
   serv.sin_addr.s_addr - inet_addr(str);
   serv.sin_port = htons(IPPORT_ECHO);

   // Open TCP socket

   if ( (sd=socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
   {
      fprintf(stderr, "echo_cli: can't open tcp socket\n");
      exit(1);
   }

   // connect echo serve

   if ( connect(sd, (LPSOCKADDR) &serv, sizeof(serv)) == SOCKET_ERROR)
   {
      fprintf(stderr, "echo_cli: can't connect to echo server\n");
      exit(1);
   }

   while ( fgets(str, MAXLINE, stdin) != NULL)
   {
      if ( send(sd, str, strlen(str)+1, 0) == SOCKET_ERROR)
      {
         fprintf(stderr, "echo_cli: send() error!\n");
         break;
      }
      if ( (n=recv(sd, str, MAXLINE, 0)) == 0)
      {
         fprintf(stderr, "echo_cli: connection closed\n");
         break;
      }
      else printf(str);
   }

   // Winsock DLL Over.

   closesocket(sd);
   WSACleanup();
}


