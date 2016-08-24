#include "stdafx.h"
#include "CServer.h"

/**
 * The main function of the program, used at startup
 * 
 * @param a_iArgumentCount - count of arguments from command line input
 * @param a_cArgumentsValues - array of arguments from command line input
 * @return - if all went well return 0, else if somthing went wrong reutn -1
 */
int main(int a_iArgumentsCount, char * a_cArgumentsValues[])
{
	//local variable of the main function result
	int _iResult = 0;

	//local variables of ip and port from cli input
	std::string _strIPAdrress = "";
	std::string _strPort = "";

	//check count of arguments
	if (3 == a_iArgumentsCount)
	{
		_strIPAdrress = a_cArgumentsValues[1];
		_strPort = a_cArgumentsValues[2];
	}

	//valid arguments from command line input
	if (CFunctions::isInputCorrect(_strIPAdrress, _strPort))
	{
		try
		{
			CServer svr(_strIPAdrress, std::stoi(_strPort));
		}
		catch (std::string exc)
		{
			CFunctions::printMessageToServerConsole(exc);
		}
	}
	else
	{
		CFunctions::printMessageToServerConsole("ERR: Somthing went wrong, check the arguments and try again.\nTIP: server.exe [IP] [PORT]");
		system("pause");
		_iResult = -1;
	}

	system("pause");
	return _iResult;
}
