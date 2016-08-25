#include "stdafx.h"
#include "CClient.h"

int main(void)
{

	CClient * _oClient = new CClient("127.0.0.1", 1234);
	delete(_oClient);
	
	system("pause");
    return 0;
}
