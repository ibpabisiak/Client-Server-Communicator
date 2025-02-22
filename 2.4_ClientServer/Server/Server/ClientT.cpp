#include "stdafx.h"
#include "ClientT.h"

int ClientT::m_iClientIDCounter = 0;

ClientT::ClientT(void) : 
	m_oClientSocket(NULL), 
	m_strClientNickname(""), 
	m_iClientID(-1), 
	m_bIsLogged(false) 
{ }

ClientT::ClientT(SOCKET a_oClientSocket) :
	m_oClientSocket(a_oClientSocket), 
	m_strClientNickname(""), 
	m_iClientID(++m_iClientIDCounter), 
	m_bIsLogged(false) 
{ }

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

bool ClientT::isClientLogged(void)
{
	return this->m_bIsLogged;
}

void ClientT::setClientLogin(bool a_bIsLogged)
{
	this->m_bIsLogged = a_bIsLogged;
}

void ClientT::setClientNickname(std::string a_strNickname)
{
	this->m_strClientNickname = a_strNickname;
}
