#include "stdafx.h"
#include "CServer.h"

/**
 * Public constructor
 * 
 * @param a_strIPAdrress - IP adrress for server
 * @param a_iPort - Port for server
 */
CServer::CServer(std::string a_strIPAdrress, int a_iPort)
{
	memset(&(this->m_oService), 0, sizeof(this->m_oService));
	
	this->m_oService.sin_family = AF_INET;
	this->m_oService.sin_addr.s_addr = inet_addr(a_strIPAdrress.c_str());
	this->m_oService.sin_port = htons(a_iPort);
	
	try
	{
		this->initializationWSAStartup();
		this->setMainSocket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		this->setBind(this->m_oService);
		this->setListen(QUEUE_LISTEN_LIMIT);
		this->runWaiterThread();
	}
	catch (std::string exc)
	{
		throw exc;
	}
}

CServer::~CServer(void) 
{ 
	this->m_oWaiterThread.join();
}

bool CServer::initializationWSAStartup(void)
{
	int result = WSAStartup(MAKEWORD(2, 2), &(this->m_oWSAData));
	
	if (NO_ERROR != result)
	{
		std::string _strException = "WSAStartup() Initialization failed";
		throw _strException;
	}

	return true;
}

bool CServer::setMainSocket(int a_iAfInet, int a_iSockStream, int a_iIprotoTcp)
{
	SOCKET _oMainSocket = socket(a_iAfInet, a_iSockStream, a_iIprotoTcp);
	if (INVALID_SOCKET == _oMainSocket)
	{
		std::string _strException = "SetMainSocket() failed";
		throw _strException;
	}
	
	this->m_oMainSocket = _oMainSocket;
	return true;
}

bool CServer::setBind(sockaddr_in & a_oServiceInfo)
{
	if (SOCKET_ERROR == bind(this->m_oMainSocket, (SOCKADDR *)& a_oServiceInfo, sizeof(a_oServiceInfo)))
	{
		std::string _strException = "SetBind() failed";
		throw _strException;
	}

	return true;
}

bool CServer::setListen(int a_iQueueLimit)
{
	if (SOCKET_ERROR == listen(this->m_oMainSocket, a_iQueueLimit))
	{
		std::string _strException = "SetListen() failed";
		throw _strException;
	}

	return true;
}

void CServer::runWaiterThread(void)
{
	this->m_oWaiterThread = std::thread(&CServer::waitToConnectionThread, this);
	//this->m_oMessagesBroadcaster = std::thread(&CServer::broadcastMessagesThread, this);
}

void CServer::waitToConnectionThread(void)
{
	while(true)
	{	
		SOCKET _oAcceptedSocket = SOCKET_ERROR;
		CFunctions::printMessageToServerConsole("Waiting for a client to connect..");
		_oAcceptedSocket = accept(this->m_oMainSocket, NULL, NULL);

		if (SOCKET_ERROR != _oAcceptedSocket)
		{
			this->m_oConnectedClients.push_back(new CConnectedClient(_oAcceptedSocket));
			this->m_oConnectedClients[this->m_oConnectedClients.size() - 1]->runReceiveMessageThread();
			CFunctions::printMessageToServerConsole("Client conected..");
		}
	}
}

void CServer::broadcastMessagesThread(void)
{
	while (true)
	{
		Sleep(50);
		std::vector<std::string> _oMessages = CConnectedClient::getMessagesBuffer();
		if (_oMessages.size() > 0)
		{
			for each (CConnectedClient * _oClient in this->m_oConnectedClients)
			{
				if (_oClient->isClientConnected())
				{
					for each (std::string _strMessage in _oMessages)
					{
						CFunctions::printMessageToServerConsole(_strMessage);
						//send(_oClient->getClientInfo().getClientSocket(), _strMessage.c_str(), sizeof(_strMessage.c_str()), NULL);
					}
				}
				else
				{
					//here delete disconnected client
				}
			}
		}
	}
}

