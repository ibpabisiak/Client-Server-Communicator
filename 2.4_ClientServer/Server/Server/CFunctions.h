#pragma once
class CFunctions
{
	public:
		static void printMessageToServerConsole(std::string a_sMsg);
		static std::string formatClientMessage(int, std::string, std::string);
		static bool isInputCorrect(std::string &, std::string &);
};
