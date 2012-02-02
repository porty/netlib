#pragma once

#include "AtomicInt32.h"

#include <string>

namespace Net
{
	class IP4Address
	{
	public:
		const static unsigned long LOCALHOST = 0x7f000001;

		static IP4Address resolve(const char * const hostname);

		IP4Address(): address(0) {}
		IP4Address(unsigned long address): address(address) {}

		unsigned long get() const { return address; }
		void set(unsigned long value) { address = value; }

		std::string toString() const;
	private:
		unsigned long address;
	};

	class TcpSocket
	{
	public:
		typedef unsigned int handle_t;

		TcpSocket();
		TcpSocket(const TcpSocket & rhs);
		~TcpSocket();

		bool isOk() const { return isOk(handle); }

		void connect(const IP4Address & address, unsigned short port);
		void bind(const IP4Address & address, unsigned short port);
		void listen();
		//void accept(IP4Address & address, unsigned short & port, TcpSocket & client);
		TcpSocket accept(IP4Address & address, unsigned short & port);
		void close();

		int read(char * const buffer, int size);
		int write(const char * const buffer, int size);
		void writeAll(const char * const buffer, int size);
	private:
		TcpSocket(handle_t handle);
		TcpSocket & operator=(const TcpSocket & rhs);
		handle_t handle;
		OS::AtomicInt32 * handleCounter;

		void checkError(int error, const char * const message);
		static bool isOk(handle_t handle);
	};
}
