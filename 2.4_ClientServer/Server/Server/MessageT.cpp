#include "stdafx.h"
#include "MessageT.h"

MessageT::MessageT(int a_iSenderID, std::string a_strSenderNickname, std::string a_strMessage) :
	m_iSenderID(a_iSenderID), m_strSenderNickname(a_strSenderNickname), m_strMessage(a_strMessage) { }

MessageT::~MessageT(void) { }

int MessageT::getSenderID(void)
{
	return this->m_iSenderID;
}

std::string MessageT::getSenderNickname(void)
{
	return this->m_strSenderNickname;
}

std::string MessageT::getMessage(void)
{
	return this->m_strMessage;
}

std::string MessageT::getFormattedMessage(void)
{
	std::string _strFormattedMessage = this->formatClientMessage(
		this->getSenderID(), 
		this->getSenderNickname(), 
		this->getMessage()
	);
	
	return _strFormattedMessage;
}

/**
* Method to format messages from clients to clients
*
* @param string a_iClientID - client unique id
* @param string a_strMessage - message from client
* @return - formatted message to send
*/
std::string MessageT::formatClientMessage(int a_iClientID, std::string a_strClientNickname, std::string a_strMessage)
{
	std::string _strOutputMessage;
	std::stringstream _ssClientPrefix;

	_ssClientPrefix << "> " << a_strClientNickname << "[ID: " << a_iClientID << "]: ";

	if (a_strMessage.length() > (MAX_BUFFER_SIZE - _ssClientPrefix.str().length()))
	{
		_strOutputMessage = "* Message is too long!";
	}
	else
	{
		char _cMessageBuffer[MAX_BUFFER_SIZE];
		sprintf(_cMessageBuffer, "%s%s", _ssClientPrefix.str().c_str(), a_strMessage.c_str());
		_strOutputMessage = _cMessageBuffer;
	}

	return _strOutputMessage;
}