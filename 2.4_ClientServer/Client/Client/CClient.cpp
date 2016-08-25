#include "stdafx.h"
#include "CClient.h"
#include <csignal>

CClient::CClient(std::string a_strIPAdrress, int a_iPort) {
	memset(&(this->m_oService), 0, sizeof(this->m_oService));
	memset(&(this->m_oMainSocket), 0, sizeof(this->m_oMainSocket));

	this->m_oService.sin_family = AF_INET;
	this->m_oService.sin_addr.s_addr = inet_addr(a_strIPAdrress.c_str());
	this->m_oService.sin_port = htons(a_iPort);

	try
	{
		this->m_initializationWSAStartup();
		this->m_setMainSocket();
		this->m_connectToServer();
	}
	catch (std::string exc)
	{
		std::cout << exc << std::endl;
	}
}

CClient::~CClient() 
{
	std::string strGoodByeMessage = "Ochodzê!";
	send(this->m_oMainSocket, strGoodByeMessage.c_str(), sizeof(strGoodByeMessage.c_str()), NULL);

	closesocket(this->m_oMainSocket);
	if (this->m_oReceiveThread.joinable())
	{
		this->m_oReceiveThread.join();
	}
}

bool CClient::m_initializationWSAStartup(void)
{
	int result = WSAStartup(MAKEWORD(2, 2), &(this->m_oWsaData));

	if (result != NO_ERROR)
	{
		std::string _strExceptionMessage = "* WSAStartup() Initialization failed";
		throw _strExceptionMessage;
	}

	return true;
}

bool CClient::m_setMainSocket(void)
{
	SOCKET mainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (mainSocket == INVALID_SOCKET)
	{
		std::string _strExceptionMessage = "* SetMainSocket() failed";
		throw _strExceptionMessage;
	}
	this->m_oMainSocket = mainSocket;

	return true;
}

bool CClient::m_connectToServer(void)
{
	if (connect(this->m_oMainSocket, (SOCKADDR *) & this->m_oService, sizeof(this->m_oService)) == SOCKET_ERROR)
	{
		std::string _strExceptionMessage = "* Connect() failed";
		throw _strExceptionMessage;
	}
	this->m_oReceiveThread = std::thread(&CClient::m_ReceiverMessagesThread, this);
	this->m_messagesBroadcaster();

	return true;
}

void CClient::m_messagesBroadcaster(void)
{
	std::regex _oWhitespaceConditionInRegex("\\s+");

	while (true)
	{
		char _cNewMessageBuffer[MAX_BUFFER_SIZE];
		std::cin.getline(_cNewMessageBuffer, sizeof(_cNewMessageBuffer));


		if (strcmp(_cNewMessageBuffer, "/q") == 0)
		{
			break;
		}
		else if (strcmp(_cNewMessageBuffer, "/kill") == 0)
		{
			for (int i = 1; i < 1000; i++)
			{
				std::stringstream _ssTemporaryContainerOfConvertIntToString;
				_ssTemporaryContainerOfConvertIntToString << i;
				std::string _strConvertedIntToString = _ssTemporaryContainerOfConvertIntToString.str();

				Sleep(5);
				send(this->m_oMainSocket, _strConvertedIntToString.c_str(), sizeof(_strConvertedIntToString.c_str()), NULL);				
			}
		}
		else if (std::regex_match(_cNewMessageBuffer, _oWhitespaceConditionInRegex))
		{
			std::cout << "* You can not send only whitespaces! Try again moron.." << std::endl;
		}
		else
		{
			try
			{
				if (-1 == send(this->m_oMainSocket, _cNewMessageBuffer, sizeof(_cNewMessageBuffer), NULL))
				{
					throw std::exception();
				}

			}
			catch (...)
			{
				break;
			}
		}
	}
}

void CClient::m_ReceiverMessagesThread(void)
{
	char _buffer[MAX_BUFFER_SIZE];
	while (true)
	{
		memset(&_buffer, 0, MAX_BUFFER_SIZE);
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
