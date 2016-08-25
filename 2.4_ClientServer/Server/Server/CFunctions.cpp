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
