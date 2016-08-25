#pragma once
class MessageT
{
	public:
		MessageT(int, std::string, std::string);
		~MessageT(void);

		int getSenderID(void);
		std::string getSenderNickname(void);
		std::string getMessage(void);
		std::string getFormattedMessage(void);

	private:
		int m_iSenderID;
		std::string m_strSenderNickname;
		std::string m_strMessage; 
		std::string formatClientMessage(int, std::string, std::string);
};
