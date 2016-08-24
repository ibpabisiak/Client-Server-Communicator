#include "stdafx.h"
#include "CConnectedClient.h"
#include "ClientT.h"
#include "CFunctions.h"


std::vector<std::string> CConnectedClient::m_oMessagesBuffer;

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

std::vector<std::string> CConnectedClient::getMessagesBuffer(void)
{
	std::vector<std::string> _oCopyOfBuffer = CConnectedClient::m_oMessagesBuffer;
	m_oMessagesBuffer.clear();


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
			
			/*this->m_oMessagesBuffer.push_back(
				CFunctions::formatClientMessage(
					this->m_oClientInfo.getCliendID(),
					this->m_oClientInfo.getClientNickname(),
					_cMessageBuffer)
			);*/

			//debug code to delete
			CFunctions::printMessageToServerConsole(
				CFunctions::formatClientMessage(
					this->m_oClientInfo.getCliendID(),
					this->m_oClientInfo.getClientNickname(),
					_cMessageBuffer)
			);
		}

		//for debug, cout msgs in svr console
		//std::cout << CFunctions::formatClientMsg(a_iClientID, _buffer) << std::endl;

		//st¹d do bufora, a potem rozsy³anie tego w server.cpp i jesteœmy w domu :)
		//kod poni¿ej to rozwi¹¿e

		/*for each (auto _client in this->m_oConnectedClients)
		{
			if (_client.getCliendID() != a_iClientID)
			{
				char _cMsgToSend[MAX_BUFFER_SIZE];
				strcpy(_cMsgToSend, CFunctions::formatClientMsg(a_iClientID, _buffer).c_str());
				send(_client.getClientSocket(), _cMsgToSend, sizeof(_cMsgToSend), 0);
			}
		}*/
	}
}
