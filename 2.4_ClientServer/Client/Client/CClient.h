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

	bool m_InitializationWSAStartup(void);
	bool m_SetMainSocket(void);
	bool m_Connect(void);
	void m_SenderMessages(void);
	void m_ReceiverMessagesThread(void);

};
