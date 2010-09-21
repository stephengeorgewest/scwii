#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gccore.h>
#include <network.h>

#define MULTICAST_GROUP "224.0.0.1"
#define MULTICAST_PORT 4000

#define MSGBUFSIZE 1024

#include "NetworkConsole.h"

NetworkConsole::NetworkConsole()
{
	printf("Initializing Network\n");
	
	int Connect = net_init();
	if(Connect != 0)
	{
		printf("Net_Init() failed.\n");
		//printf("Press A to retry.\n");
		//waitfor(PAD_BUTTON_A);
	}
	else
	{
		printf("Net_Init() successful.\n\n");
		
		printf("Net_Socket Initializing.\n");
		netSocket = net_socket(AF_INET, SOCK_DGRAM, 0);
		
		if(netSocket == INVALID_SOCKET)
		{
			printf("Net_Socket Initialization failed.\n");
			//printf("Press A to retry.\n");
		}
		else
		{
			printf("Net_Socket Initialized.\n\n");
			
			printf("Establishing server on port %d.\n",MULTICAST_PORT);
			
			sin.sin_port=htons(MULTICAST_PORT);
			sin.sin_addr.s_addr=inet_addr(MULTICAST_GROUP);//replace with multicast destination
			sin.sin_family=AF_INET;
			if(net_bind(netSocket,(sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR)
			{
				printf("Failed to bind.\n");
				//printf("Press A to retry.\n");
				//waitfor(PAD_BUTTON_A);
				//goto error;
			}
			else
			{
				printf("Server established!\n");
				if(net_connect(netSocket,(sockaddr*)&sin, sizeof(sin))==-1)
				{
					printf("net_connect Erron\n");
				}
			}
		}
	}
}
NetworkConsole::~NetworkConsole()
{
	net_close(netSocket);
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
NetworkConsole::send_message(const char * message)
{
	//int i=0;
	//for(i=0; i<MSGBUFSIZE; i++)
	//	message[i]=0;
	//
	//puts("Send a message:");
	//fgets(message, MSGBUFSIZE, stdin);
	int num_sent = net_send(netSocket,message,strlen(message),0);
	//s32 net_sendto(s32 s,const void *data,s32 len,u32 flags,struct sockaddr *to,socklen_t tolen);
	if (num_sent < 0)
	{
		puts("sendto error");
		return num_sent;
	}
	//printf("Message sent %d bytes:%s\n\n", num_sent, message);
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
