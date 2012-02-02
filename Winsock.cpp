#include "Winsock.h"
#include <WinSock2.h>

using namespace Net;

Winsock::Winsock()
{
	WSAData data;
	WSAStartup(MAKEWORD(2, 2), &data);
}

Winsock::~Winsock()
{
	WSACleanup();
}
