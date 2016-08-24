#pragma once
class MessageT
{
	public:
		MessageT(int, std::string);
		~MessageT();

		int getSenderID(void);
		std::string getMessage(void);

	private:
		int m_iSenderID;
		std::string m_strMessage;
};

