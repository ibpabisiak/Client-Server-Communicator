#include "stdafx.h"
#include "CClient.h"
#include <csignal>

CClient::CClient(std::string a_sIp, int a_iPort) {
	memset(&(this->m_oService), 0, sizeof(this->m_oService));
	memset(&(this->m_oMainSocket), 0, sizeof(this->m_oMainSocket));

	this->m_oService.sin_family = AF_INET;
	this->m_oService.sin_addr.s_addr = inet_addr(a_sIp.c_str());
	this->m_oService.sin_port = htons(a_iPort);

	try
	{
		this->m_InitializationWSAStartup();
		this->m_SetMainSocket();
		this->m_Connect();
	}
	catch (std::string e)
	{
		std::cout << e << std::endl;
	}
}

CClient::~CClient() 
{
	std::string strGoodByeMessage = "Saynoara!";
	send(this->m_oMainSocket, strGoodByeMessage.c_str(), sizeof(strGoodByeMessage.c_str()), NULL);
	closesocket(this->m_oMainSocket);
	if(this->m_oReceiveThread.joinable())
		this->m_oReceiveThread.join();
}

bool CClient::m_InitializationWSAStartup(void)
{
	int result = WSAStartup(MAKEWORD(2, 2), &(this->m_oWsaData));
	if (result != NO_ERROR)
	{
		std::string _sException = "* WSAStartup() Initialization failed";
		throw _sException;
	}
	return true;
}

bool CClient::m_SetMainSocket(void)
{
	SOCKET mainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mainSocket == INVALID_SOCKET)
	{
		std::string _sException = "* SetMainSocket() failed";
		throw _sException;
	}

	this->m_oMainSocket = mainSocket;
	return true;
}

bool CClient::m_Connect(void)
{
	if (connect(this->m_oMainSocket, (SOCKADDR *) & this->m_oService, sizeof(this->m_oService)) == SOCKET_ERROR)
	{
		std::string _sException = "* Connect() failed";
		throw _sException;
	}
	this->m_oReceiveThread = std::thread(&CClient::m_ReceiverMessagesThread, this);
	this->m_SenderMessages();

	return true;
}

void CClient::m_SenderMessages(void)
{
	while (true)
	{
		char _buffer[MAX_BUFFER_SIZE];
		std::cin.getline(_buffer, sizeof(_buffer));

		try
		{
			if (-1 == send(this->m_oMainSocket, _buffer, sizeof(_buffer), NULL))
			{
				throw std::exception();
			}

		}
		catch (...)
		{
			break;
		}

		if (strcmp(_buffer, "/q") == 0)
		{
			std::cout << "Hehe: " << _buffer << std::endl;
			break;
		}

		if (strcmp(_buffer, "/kill") == 0)
		{
			for (int i = 1; i < 1000; i++)
			{
				std::stringstream ss;
				ss << i;
				std::string temp = ss.str();
				Sleep(5);
				send(this->m_oMainSocket, temp.c_str(), sizeof(temp.c_str()), NULL);
				
			}
		}

	}
}

void CClient::m_ReceiverMessagesThread(void)
{
	char _buffer[MAX_BUFFER_SIZE];
	while (true)
	{
		memset(&_buffer, 0, sizeof(_buffer));
		try
		{
			if (-1 == recv(this->m_oMainSocket, _buffer, sizeof(_buffer), 0))
			{
				throw std::exception();
			}
			std::cout << _buffer << std::endl;
		}
		catch (...)
		{
			break;
		}
	}
}
