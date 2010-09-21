#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gccore.h>
#include <network.h>
#include <unistd.h>

#define MULTICAST_GROUP "224.1.1.1"
#define MULTICAST_PORT 4000

#define MSGBUFSIZE 1024
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gccore.h>
#include <network.h>
#include "NetworkConsole.h"

struct NetworkConsole::NetworkConsolePrivateData
{
	s32 netSocket;
	sockaddr_in sin;
}Data;


NetworkConsole::NetworkConsole():Data(new NetworkConsolePrivateData)
{};
void
NetworkConsole::Init()
{
	printf("Initializing Network\n");
	
	int Connect = net_init();
	if(Connect != 0)
	{
		printf("Net_Init() failed.\n");
	}
	else
	{
		printf("Net_Init() successful.\n\n");
		
		printf("Net_Socket Initializing.\n");
		Data->netSocket = net_socket(AF_INET, SOCK_DGRAM, 0);
		
		if(Data->netSocket == INVALID_SOCKET)
		{
			printf("Net_Socket Initialization failed.\n");
		}
		else
		{
			printf("Net_Socket Initialized.\n\n");
			printf("Establishing server on port %d.\n",MULTICAST_PORT);
			
			Data->sin.sin_port=htons(MULTICAST_PORT);
			Data->sin.sin_addr.s_addr=inet_addr(MULTICAST_GROUP);//replace with multicast destination
			Data->sin.sin_family=AF_INET;
			if(net_bind(Data->netSocket,(sockaddr*)&(Data->sin), sizeof(Data->sin)) == SOCKET_ERROR)
			{
				printf("Failed to bind.\n");
			}
			else
			{
				printf("Server established!\n");
				if(net_connect(Data->netSocket,(sockaddr*)&(Data->sin), sizeof(Data->sin))==-1)
				{
					printf("net_connect Error\n");
				}
			}
		}
	}
}
NetworkConsole::~NetworkConsole()
{
	net_close(Data->netSocket);
	delete(Data);
}
/*
int NetworkConsole::setup_non_blocking()
{
	int flags = fcntl(fd, F_GETFL);
	flags |= O_NONBLOCK;
	fcntl(fd, F_SETFL, flags);
	return 0;// no errors checked yet
}
*/
int
NetworkConsole::sendMessage(const char * message)
{
	int num_sent = net_send(Data->netSocket,message,strlen(message),0);
	//usleep(5000);
	if (num_sent < 0)
	{
		puts("sendto error");
		return num_sent;
	}
	return num_sent;
	
}
/*
int NetworkConsole::recieve_message(struct MulticastStruct*RN, char * msgbuf_in)
{
	int i=0;
        for(i=0; i<1024; i++)
                msgbuf_in[i]=0;
	int addrlen = sizeof(addr);
	int nbytes=recvfrom(fd, msgbuf_in, MSGBUFSIZE, 0, (struct sockaddr *) &addr, &addrlen );
	if ( nbytes < 0)
	{
		puts("recvfrom error");
		return nbytes;
	}
	//printf("Recieved Message %d bytes long:%s\n", nbytes, msgbuf_in);
	return nbytes;
}*/
