#include "stdafx.h"
#include "CFunctions.h"

/**
 * Static method to print the message on the server console
 *
 * @param string a_strMessage - message to print
 */
void CFunctions::printMessageToServerConsole(std::string a_strMessage)
{
	std::cout << "* " << a_strMessage << std::endl;
}

/**
 * Static method to format messages from clients to clients
 *
 * @param string a_iClientID - client unique id
 * @param string a_strMessage - message from client
 * @return - formatted message to send
 */
std::string CFunctions::formatClientMessage(int a_iClientID, std::string a_strClientNickname, std::string a_strMessage)
{
	std::string _strOutputMessage;
	std::stringstream _ssClientPrefix;

	_ssClientPrefix << "> " << a_strClientNickname << "[ID: " << a_iClientID << "]: ";

	if (a_strMessage.length() > (MAX_BUFFER_SIZE - _ssClientPrefix.str().length()))
	{
		_strOutputMessage = "* Message is too long!";
	}
	else 
	{
		char _cMessageBuffer[MAX_BUFFER_SIZE];
		sprintf(_cMessageBuffer, "%s%s", _ssClientPrefix.str().c_str(), a_strMessage.c_str());
		_strOutputMessage = _cMessageBuffer;
	}

	return _strOutputMessage;
}


/**
 * Static method to valid input from command line
 *
 * @param string a_strIPAdrress - ip address from command line
 * @param string a_strPort - port from command line
 * @return - true if the arguments are correct, false if are not
 */
bool CFunctions::isInputCorrect(std::string & a_strIPAdrress, std::string & a_strPort)
{
	std::regex _oInternetProtocolPattern("(\\d{1,3}(\\.\\d{1,3}){3})");
	std::regex _oPortPattern("\\d{1,5}");

	bool _bInternetProtocolMatchResult = std::regex_match(a_strIPAdrress, _oInternetProtocolPattern);
	bool _bPortMatchResult = std::regex_match(a_strPort, _oPortPattern);

	return (_bInternetProtocolMatchResult && _bPortMatchResult);
}
