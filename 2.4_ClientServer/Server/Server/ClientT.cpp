#include "stdafx.h"
#include "ClientT.h"

int ClientT::m_iClientIDCounter = 0;

ClientT::ClientT(void) : 
	m_oClientSocket(NULL), m_strClientNickname(""), m_iClientID(-1) { }

ClientT::ClientT(SOCKET a_oClientSocket) :
	m_oClientSocket(a_oClientSocket), m_strClientNickname(""), m_iClientID(++m_iClientIDCounter) { }

ClientT::~ClientT() { }

SOCKET ClientT::getClientSocket(void)
{
	return this->m_oClientSocket;
}

std::string ClientT::getClientNickname(void)
{
	return this->m_strClientNickname;
}

int ClientT::getCliendID(void)
{
	return this->m_iClientID;
}

void ClientT::setClientNickname(std::string a_strNickname)
{
	this->m_strClientNickname = a_strNickname;
}
