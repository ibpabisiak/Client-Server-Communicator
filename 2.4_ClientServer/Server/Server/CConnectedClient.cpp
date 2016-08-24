#include "stdafx.h"
#include "CConnectedClient.h"
#include "ClientT.h"
#include "MessageT.h"
#include "CFunctions.h"


std::vector<MessageT> CConnectedClient::m_oMessagesBuffer;
std::condition_variable CConnectedClient::m_oConnectedClientsCV;
std::mutex CConnectedClient::m_oConnectedClientsMutex;
bool CConnectedClient::m_bReady = true;

CConnectedClient::CConnectedClient(SOCKET a_oClientSocket) :
	m_oClientInfo(ClientT(a_oClientSocket)), m_bIsClientConnected(true) { }

CConnectedClient::~CConnectedClient() { }

ClientT CConnectedClient::getClientInfo(void)
{
	return this->m_oClientInfo;
}

void CConnectedClient::runReceiveMessageThread(void)
{
	this->m_oClientThread = std::thread(&CConnectedClient::receiveMessageThread, this);
	send(this->m_oClientInfo.getClientSocket(), WELCOME_MSG, sizeof(WELCOME_MSG), 0);
}

bool CConnectedClient::isClientConnected(void)
{
	return this->m_bIsClientConnected;
}

std::vector<MessageT> CConnectedClient::getMessagesBuffer(void)
{
	std::unique_lock<std::mutex> _oMutex(CConnectedClient::m_oConnectedClientsMutex);
	CConnectedClient::m_oConnectedClientsCV.wait(_oMutex, [] { return CConnectedClient::m_bReady; CConnectedClient::m_bReady = false; });

	std::vector<MessageT> _oCopyOfBuffer = CConnectedClient::m_oMessagesBuffer;
	m_oMessagesBuffer.clear();

	CConnectedClient::m_bReady = true;
	_oMutex.unlock();
	CConnectedClient::m_oConnectedClientsCV.notify_all();

	return _oCopyOfBuffer;
}

void CConnectedClient::receiveMessageThread(void)
{
	bool _bIsEnteredName = false;
	while (true)
	{
		char _cMessageBuffer[MAX_BUFFER_SIZE];

		memset(&_cMessageBuffer, 0, sizeof(_cMessageBuffer));

		//rozlaczanie sie zrob
		try
		{
			if (-1 == recv(this->m_oClientInfo.getClientSocket(), _cMessageBuffer, sizeof(_cMessageBuffer), 0))
			{
				throw std::exception();
			}

		}
		catch (...)
		{
			std::cout << "rozlaczyl sie glab jebany" << std::endl;
			this->m_bIsClientConnected = false;
			break;
		}

		if (!_bIsEnteredName)
		{
			this->m_oClientInfo.setClientNickname(_cMessageBuffer);
			std::string _strAfterNicknameMessage = "Thanks! Your nickname is " + this->m_oClientInfo.getClientNickname();
			send(this->m_oClientInfo.getClientSocket(), _strAfterNicknameMessage.c_str(), sizeof(_strAfterNicknameMessage), NULL);
			_bIsEnteredName = true;
		}
		else
		{
			if (0 == strcmp(_cMessageBuffer, "") || 0 == strcmp(_cMessageBuffer, "/q"))
			{
				this->m_bIsClientConnected = false;
				break;
			}
			
			std::unique_lock<std::mutex> _oMutex(CConnectedClient::m_oConnectedClientsMutex);
			CConnectedClient::m_oConnectedClientsCV.wait(_oMutex, [] { return CConnectedClient::m_bReady; CConnectedClient::m_bReady = false; });

			this->m_oMessagesBuffer.push_back(
				MessageT(
					this->getClientInfo().getCliendID(),
					CFunctions::formatClientMessage(
						this->m_oClientInfo.getCliendID(),
						this->m_oClientInfo.getClientNickname(),
						_cMessageBuffer)
					)
			);

			////debug code to delete
			//CFunctions::printMessageToServerConsole(
			//	CFunctions::formatClientMessage(
			//		this->m_oClientInfo.getCliendID(),
			//		this->m_oClientInfo.getClientNickname(),
			//		_cMessageBuffer)
			//);

			CConnectedClient::m_bReady = true;
			_oMutex.unlock();
			CConnectedClient::m_oConnectedClientsCV.notify_all();

		}
	}
}
