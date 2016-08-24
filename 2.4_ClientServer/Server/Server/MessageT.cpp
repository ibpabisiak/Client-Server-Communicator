#include "stdafx.h"
#include "MessageT.h"

MessageT::MessageT(int a_iSenderID, std::string a_strMessage) :
	m_iSenderID(a_iSenderID), m_strMessage(a_strMessage) { }

MessageT::~MessageT() { }

int MessageT::getSenderID(void)
{
	return this->m_iSenderID;
}

std::string MessageT::getMessage(void)
{
	return this->m_strMessage;
}
