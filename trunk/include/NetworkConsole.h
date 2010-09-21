#ifndef NetworkConsole_h
#define NetworkConsole_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gccore.h>
#include <network.h>

class NetworkConsole
{
	s32 netSocket;
	sockaddr_in sin;
	
	public:
		NetworkConsole();
		~NetworkConsole();
		int send_message(const char * message);
};

#endif