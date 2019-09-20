#include "Server.h"
#include <conio.h>
#include <stdio.h>

int main()
{
	int serverPort = 82716;
	{
		Server* serverHandle = new Server(serverPort);
		printf("Server setup and listening on port %d. \n", serverPort);

		do
		{
			unsigned int clientID = serverHandle->acceptNewClients();
			if (clientID > 0)
			{
				printf("New connection from IP %s. (ID %d) of %d\n", serverHandle->sessions[clientID]->IPAddress, clientID,++(serverHandle->connectedClients));
			}

			// Process our existing clients
			for (auto it = serverHandle->sessions.cbegin(); it != serverHandle->sessions.cend();)
			{
				Client* con = serverHandle->sessions[it->first];
				//char* clientName = new char[NETWORK_BUFFER_SIZE];

				//for (int i = 0;i < serverHandle->connectedClients;i++)
				//{
				//	con->SendNetworkMessage();
				//}

				//int clientNameSie = sprintf_s(clientName, NETWORK_BUFFER_SIZE, "Welcome %d (%s)!", con->connectionID, con->IPAddress);
				//con->SendNetworkMessage(clientName, clientNameSie);
				bool deleteConnection = false;

				if (con->ReadNetworkMessageSize() > 0)
				{
					if (con->NetworkMessageIsComplete('\r'))
					{
						char* message = con->GetNetworkMessage();
						printf("New message from client %d: %s \n", con->connectionID, message);
						if (strcmp(message, "exit") == 0)
						{
							printf("Client %d: requested disconnect.", con->connectionID);
							serverHandle->connectedClients--;
							con->Shutdown();
							deleteConnection = true;
						}
						//if(strcmp(message, "/nick") == 0)

						int msgLength = -1; while (message[++msgLength] != '\0' && msgLength < NETWORK_BUFFER_SIZE);
						char* relayMessage = new char[NETWORK_BUFFER_SIZE];
						if (strstr(message, "echo") != nullptr)
						{
							strncpy_s(relayMessage, msgLength - 3, message + 4, msgLength - 4);
							con->SendNetworkMessage(message, msgLength);
						}
						else
						{
							int relayMessageSize = sprintf_s(relayMessage, NETWORK_BUFFER_SIZE, "User %d (%s): %s\n\r\n\r", con->connectionID, con->IPAddress, message);
							for (auto subIt = serverHandle->sessions.cbegin(); subIt != serverHandle->sessions.cend();++subIt)
								serverHandle->sessions[subIt->first]->SendNetworkMessage(relayMessage, relayMessageSize);
						}
					}
				}

				if (deleteConnection)
				{
					serverHandle->sessions.erase(it++);
				}
				else
				{
					++it;
				}
			}
		} while (serverHandle->connectedClients >= 0);
	}
}

