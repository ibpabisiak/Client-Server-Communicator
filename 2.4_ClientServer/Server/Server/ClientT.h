#pragma once
class ClientT
{
	public:
		ClientT(void);
		ClientT(SOCKET);
		~ClientT();

		SOCKET getClientSocket(void);
		std::string getClientNickname(void);
		int getCliendID(void);
		bool isClientLogged(void);
		void setClientLogin(bool);
		void setClientNickname(std::string);

	private:
		static int m_iClientIDCounter;

		SOCKET m_oClientSocket;
		std::string m_strClientNickname;
		int m_iClientID;
		bool m_bIsLogged;
};

