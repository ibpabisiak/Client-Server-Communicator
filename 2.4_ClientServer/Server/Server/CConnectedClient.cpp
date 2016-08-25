#include "stdafx.h"
#include "CConnectedClient.h"
#include "ClientT.h"
#include "MessageT.h"
#include "CFunctions.h"


std::vector<MessageT> CConnectedClient::m_oMessagesBuffer;

std::condition_variable CConnectedClient::m_oConnectedClientsCV;
std::mutex CConnectedClient::m_oConnectedClientsMutex;
bool CConnectedClient::m_bIsMessagesBufferAvailable = true;

std::condition_variable CConnectedClient::m_oBroadcasterThreadConditionVariable;
std::mutex CConnectedClient::m_oBroadcasterThreadMutex;
bool CConnectedClient::m_bIsNewMessages = false;

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
	CConnectedClient::m_oConnectedClientsCV.wait(_oMutex, [] { return CConnectedClient::m_bIsMessagesBufferAvailable; CConnectedClient::m_bIsMessagesBufferAvailable = false; });

	std::vector<MessageT> _oCopyOfBuffer = CConnectedClient::m_oMessagesBuffer;
	m_oMessagesBuffer.clear();

	CConnectedClient::m_bIsMessagesBufferAvailable = true;
	_oMutex.unlock();
	CConnectedClient::m_oConnectedClientsCV.notify_all();

	return _oCopyOfBuffer;
}

void CConnectedClient::receiveMessageThread(void)
{
	bool _bIsNicknameEntered = false;
	while (true)
	{
		char _cNewMessageBuffer[MAX_BUFFER_SIZE];
		memset(&_cNewMessageBuffer, 0, sizeof(_cNewMessageBuffer));

		try
		{
			if (-1 == recv(this->m_oClientInfo.getClientSocket(), _cNewMessageBuffer, sizeof(_cNewMessageBuffer), 0))
			{
				throw std::exception();
			}
		}
		catch (...)
		{
			this->m_bIsClientConnected = false;
			break;
		}

		if (0 == strcmp(_cNewMessageBuffer, "") || 0 == strcmp(_cNewMessageBuffer, "/q"))
		{
			this->m_bIsClientConnected = false;
			break;
		}

		if (!_bIsNicknameEntered)
		{
			this->m_oClientInfo.setClientNickname(_cNewMessageBuffer);
			this->m_oClientInfo.setClientLogin(true);
			std::string _strAfterNicknameMessage = "> Thanks! Your nickname is " + this->m_oClientInfo.getClientNickname();
			send(
				this->m_oClientInfo.getClientSocket(), 
				_strAfterNicknameMessage.c_str(), 
				(sizeof(_strAfterNicknameMessage.c_str())/4) * _strAfterNicknameMessage.length(), 
				NULL
			);
			_bIsNicknameEntered = true;
		}
		else
		{
			std::unique_lock<std::mutex> _oReceiverLockMutex(CConnectedClient::m_oConnectedClientsMutex);
			CConnectedClient::m_oConnectedClientsCV.wait(_oReceiverLockMutex, [] { return CConnectedClient::m_bIsMessagesBufferAvailable; CConnectedClient::m_bIsMessagesBufferAvailable = false; });

			this->m_oMessagesBuffer.push_back(
				MessageT(
					this->getClientInfo().getCliendID(),
					this->getClientInfo().getClientNickname(),
					_cNewMessageBuffer
				)
			);

			CConnectedClient::m_bIsMessagesBufferAvailable = true;
			
			_oReceiverLockMutex.unlock();
			CConnectedClient::m_oConnectedClientsCV.notify_all();

			std::unique_lock<std::mutex> _oBroadcasterLockMutex(CConnectedClient::m_oBroadcasterThreadMutex);
			_oBroadcasterLockMutex.unlock();
			CConnectedClient::m_oBroadcasterThreadConditionVariable.notify_one();
		}
	}
}
