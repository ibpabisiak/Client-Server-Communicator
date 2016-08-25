#pragma once
class CClient
{
public:
	CClient(std::string a_sIp, int a_iPort);
	~CClient();

private:
	sockaddr_in m_oService;
	SOCKET m_oMainSocket;
	WSADATA m_oWsaData;
	std::thread m_oReceiveThread;

	bool m_initializationWSAStartup(void);
	bool m_setMainSocket(void);
	bool m_connectToServer(void);
	void m_messagesBroadcaster(void);
	void m_ReceiverMessagesThread(void);
};
