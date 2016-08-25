#pragma once
#include "stdafx.h"
#include "ClientT.h"
#include "MessageT.h"
#include "CFunctions.h"

class CConnectedClient
{
	public:
		static std::condition_variable m_oBroadcasterThreadConditionVariable;
		static std::mutex m_oBroadcasterThreadMutex;

		CConnectedClient(SOCKET);
		~CConnectedClient();

		ClientT getClientInfo(void);
		void runReceiveMessageThread(void);
		bool isClientConnected(void);
		static std::vector<MessageT> getMessagesBuffer(void);

	private:
		//static fields
		static std::vector<MessageT> m_oMessagesBuffer;

		static std::condition_variable m_oConnectedClientsCV;
		static std::mutex m_oConnectedClientsMutex;
		static bool m_bIsMessagesBufferAvailable;

		//fields
		ClientT m_oClientInfo;
		std::thread m_oClientThread;
		bool m_bIsClientConnected;

		//methods
		void receiveMessageThread(void);
};
