#pragma once
#include "ClientT.h"
#include "CConnectedClient.h"
#include "CFunctions.h"

class CServer
{
	public:
		CServer(std::string, int);
		~CServer(void);
	
	private:
		sockaddr_in m_oService;
		WSADATA m_oWSAData;
		SOCKET m_oMainSocket;
		std::thread m_oWaiterThread;
		std::thread m_oMessagesBroadcaster;
		std::condition_variable m_oNewClientConnectedCV;
		std::mutex m_oNewClientConnectedMutex;
		std::vector<CConnectedClient *> m_oConnectedClients;
		bool m_bIsNewClientConnected;

		bool initializationWSAStartup(void);
		bool setMainSocket(int, int, int);
		bool setBind(sockaddr_in &); //if return false not forget close mainSocket!!!
		bool setListen(int);
		void runWaiterThread(void);
		void waitToConnectionThread(void);
		void broadcastMessagesThread(void);
};
