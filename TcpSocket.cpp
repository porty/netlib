#include "TcpSocket.h"
#include "UnionCast.h"

#include <iostream>

#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

using namespace Net;

void makeSocketAddress(const IP4Address & address, unsigned short port, sockaddr_in & out)
{
	memset(&out, 0, sizeof(out));
	out.sin_addr.s_addr = htonl(address.get());
	out.sin_family = AF_INET;
	out.sin_port = ntohs(port);
}

IP4Address IP4Address::resolve(const char * const hostname)
{
	hostent * host;
	host = gethostbyname(hostname);
	unsigned long firstAddress;
	if (!host)
	{
		throw std::exception();
	}
	if (host->h_addrtype == AF_INET)
	{
		firstAddress = ntohl(*reinterpret_cast<unsigned long *>(host->h_addr_list[0]));
	}
	else
	{
		throw std::exception();
	}
	return IP4Address(firstAddress);
}

std::string IP4Address::toString() const
{
	return inet_ntoa(union_cast<in_addr, unsigned long>(htonl(address)));
}

TcpSocket::TcpSocket()
	: handle(INVALID_SOCKET), handleCounter(new OS::AtomicInt32(1))
{
	std::cerr << "TcpSocket()\n";
	handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (!isOk())
	{
		throw std::exception("Failed to create socket");
	}
}

TcpSocket::TcpSocket(const TcpSocket & rhs)
	: handle(rhs.handle), handleCounter(rhs.handleCounter)
{
	std::cerr << "TcpSocket(const TcpSocket &)\n";
	++(*handleCounter);
}

TcpSocket & TcpSocket::operator=(const TcpSocket & rhs)
{
	std::cerr << "operator=(const TcpSocket &)\n";
	if (this != &rhs)
	{
		close();
		handleCounter = rhs.handleCounter;
		++(*handleCounter);
		handle = rhs.handle;
	}
	return *this;
}

TcpSocket::TcpSocket(handle_t handle)
	: handle(handle), handleCounter(new OS::AtomicInt32(1))
{
	std::cerr << "TcpSocket(handle_t)\n";
}

TcpSocket::~TcpSocket()
{
	std::cerr << "~TcpSocket()\n";
	if (isOk())
	{
		close();
	}
}

void TcpSocket::connect(const IP4Address & address, unsigned short port)
{
	sockaddr_in sockaddress;
	makeSocketAddress(address, port, sockaddress);
	int error = ::connect(handle, reinterpret_cast<sockaddr *>(&sockaddress), sizeof(sockaddress));

	checkError(error, "Failed to connect");
}

void TcpSocket::bind(const IP4Address & address, unsigned short port)
{
	sockaddr_in sockaddress;
	makeSocketAddress(address, port, sockaddress);
	int error = ::bind(handle, reinterpret_cast<sockaddr *>(&sockaddress), sizeof(sockaddress));

	checkError(error, "Failed to bind");
}

void TcpSocket::listen()
{
	if (SOCKET_ERROR == ::listen(handle, 5))
	{
		checkError(WSAGetLastError(), "Failed to listen");
	}
}

TcpSocket TcpSocket::accept(IP4Address & address, unsigned short & port)
{
	sockaddr_in sockaddress;
	int addrlen = sizeof(sockaddress);
	handle_t newsock = ::accept(handle, reinterpret_cast<sockaddr *>(&sockaddress), &addrlen);
	if (!isOk(newsock))
	{
		// should throw us straight out
		checkError(WSAGetLastError(), "Failed to accept");
	}
	address.set(sockaddress.sin_addr.s_addr);
	port = sockaddress.sin_port;

	return TcpSocket(newsock);
	//client.close();
	//client.handle = newsock;
}

void TcpSocket::close()
{
	std::cerr << "close()\n";
	if (handleCounter)
	{
		OS::AtomicInt32 & counter = *handleCounter;
		if (! --counter)
		{
			shutdown(handle, SD_SEND);
			::closesocket(handle);
			handle = INVALID_SOCKET;
			delete handleCounter;
			handleCounter = NULL;
		}
	}
}

int TcpSocket::read(char * const buffer, int size)
{
	// possible flags:
	// MSG_PEEK - peek
	// MSG_OOB - out-of-band data
	// MSG_WAITALL - only return when buffer filled or disconnected
	int retval = recv(handle, buffer, size, 0);
	if (retval == SOCKET_ERROR)
	{
		// should throw
		checkError(WSAGetLastError(), "Failed to read");
	}
	return retval;
}

int TcpSocket::write(const char * const buffer, int size)
{
	// possible flags
	// MSG_DONTROUTE - ??
	// MSG_OOB
	int retval = send(handle, buffer, size, 0);
	if (retval == SOCKET_ERROR)
	{
		// should throw
		checkError(WSAGetLastError(), "Failed to write");
	}
	return retval;
}

void TcpSocket::writeAll(const char * const buffer, int size)
{
	int sent = 0;
	int retval;
	while (sent < size)
	{
		retval = write(buffer + sent, size - sent);
		if (retval == 0)
		{
			// disconnected
			// TODO: what to do? return false?
			throw std::exception("Failed to write all");
		}
		sent += retval;
	}
}

void TcpSocket::checkError(int error, const char * const message)
{
	if (error != 0)
	{
		throw std::exception(message);
	}
}

bool TcpSocket::isOk(handle_t handle)
{
	return handle != INVALID_SOCKET;
}
