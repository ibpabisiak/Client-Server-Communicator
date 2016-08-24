#pragma once
#include "stdafx.h"
#include "ClientT.h"
#include "CFunctions.h"

class CConnectedClient
{
public:
	CConnectedClient(SOCKET);
	~CConnectedClient();

	ClientT getClientInfo(void);
	void runReceiveMessageThread(void);
	bool isClientConnected(void);
	static std::vector<std::string> getMessagesBuffer(void);

private:
	//static fields
	static std::vector<std::string> m_oMessagesBuffer;
	
	//fields
	ClientT m_oClientInfo;
	std::thread m_oClientThread;
	bool m_bIsClientConnected;

	//methods
	void receiveMessageThread(void);
};
