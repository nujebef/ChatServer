#include <winsock2.h>
#include <WS2tcpip.h>
#pragma once

#include <map>
#include "Client.h"
#pragma comment (lib, "Ws2_32.lib")

class Server
{
private:
	// This counter will increase by one for each connection, giving us our own ID for connections from clients.
	unsigned int IDCounter = 1;

	// This will be the socket handle we use to listen to incoming connections: 
	SOCKET ListenSocket;

public:
	Server(int defaultPort);
	~Server();

	// This data structure will hold all our connections in a table:
	std::map<unsigned int, Client*> sessions;

	unsigned int connectedClients = 0;

	// Listens for and accepts any new pending connections, assigns them an ID, and adds them to our table
	unsigned int acceptNewClients();
};

