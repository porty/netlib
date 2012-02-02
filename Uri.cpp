#include "Uri.h"
#include "SplitString.h"
#include "Encoding.h"

#include "UnionCast.h"

#include <iostream>
#include <list>
#include <vector>

using namespace Net;
using namespace OS;

Uri::Uri(const std::string & uri)
	: i(0), uri(uri), port(0)
{
/*
	http://bill:pass@roflware.com:80/path%20to/file.php?apple=banana&car%20rot=dingo
	host = roflware.com
	port = 80
	protocol = http
	rawPath = /path%20to/file.php
	path = /path to/file.php
	rawQuery = apple=banana&car%20rot=dingo
	query = apple=banana&car rot=dingo
	userInfo = bill:pass
	queryMap = {apple => banana, car rot => dingo}
*/
	// do protocol will increment the i to past the colon, or past the single/double slashes
	// if after the colon
	doProtocol();
//	std::cout << "protocol = " << protocol << "\n";
//	std::cout << "i is now " << i << "\n";
//	std::cout << "working on: '" << (uri.c_str() + i) << "'\n";
	if (!protocol.empty())
	{
		doUserHostAndPort();
	}
	doPath();
}

void Uri::doProtocol()
{
	for (i = 0; i < uri.length(); ++i)
	{
		char c = static_cast<char>(tolower(uri[i]));
		if (c == ':')
		{
			// can't be empty
			if (i == 0)
			{
				throw MalformedUriException("Invalid protocol: empty");
			}
			protocol = uri.substr(0, i);

			// skip the colon
			++i;

			// string http://x
			// index  01234567
			// length 12345678
			// go past at most 2 slashes
			for (int counter = 0; counter < 2; ++counter)
			{
				if (uri.length() > i + 1)
				{
					if (uri[i] == '/')
					{
						++i;
					}
					else
					{
						// not a slash
						break;
					}
				}
				else
				{
					// not long enough
					break;
				}
			}
			break;
		}
		else if (c == '/')
		{
			if (i == 0)
			{
				// no protocol at all
				break;
			}
			else
			{
				throw MalformedUriException("Invalid protocol: missing colon");
			}
		}
		else if (('a' <= c) && (c <= 'z'))
		{
			// continue..
		}
		else
		{
			throw MalformedUriException("Invalid protocol: invalid character(s)");
		}
	}

	// protocol done..
}

void Uri::doUserHostAndPort()
{
	const std::string::size_type start = i;
	std::string::size_type colon1 = 0;
	std::string::size_type colon2 = 0;
	std::string::size_type at = 0;
	for (; i < uri.length(); ++i)
	{
		char c = uri[i];
		if (c == ':')
		{
			if (i == start)
			{
				// something required before the colon
				throw MalformedUriException("Invalid hostname/userinfo");
			}
			if (colon1 == 0)
			{
				// first occurance of colon
				colon1 = i;
			}
			else if (at == 0)
			{
				// second colon, but no @ - bad user info
				throw new MalformedUriException("Invalid user information");
			}
			else
			{
				// they have specified user info and port info
				colon2 = i;
			}
		}
		else if ((colon1 > 0) && (c == '@'))
		{
			at = i;
		}
		else if (c == '/')
		{
			if (colon1 && at && colon2)
			{
				// user:pass@hostname:port ...
				userInfo = uri.substr(start, at - start);
				host = uri.substr(at + 1, colon2 - at - 1);
				port = getPortFromString(uri.substr(colon2 + 1, i - 1));
				if (!port)
				{
					throw MalformedUriException("Invalid port: either 0 or non-numeric");
				}
				break;
			}
			else if (colon1 && at && !colon2)
			{
				// user:pass@ostname...
				userInfo = uri.substr(start, at - start);
				host = uri.substr(at + 1, i - at - 1);
				port = getDefaultPort();
				break;
			}
			else if (colon1 && !at && !colon2)
			{
				// hostname:port...
				host = uri.substr(start, colon1 - start);
				port = getPortFromString(uri.substr(colon1 + 1, i - colon1 - 1));
				break;
			}
			else if (!colon1 && !at && !colon2)
			{
				// hostname
				host = uri.substr(start, i - start);
				port = getDefaultPort();
				break;
			}
		}
		else
		{
			// continue
		}
	}
}

void Uri::doPath()
{
	char c;
	if (i + 1 >= uri.length())
	{
		return;
	}
	else if ((c = uri[i]) != '/')
	{
		return;
	}

	std::string::size_type start = i;

	std::string::size_type qmark = uri.find_first_of('?', i);
	if (qmark == std::string::npos)
	{
		rawPath = uri.substr(start);
	}
	else
	{
		rawPath = uri.substr(start, qmark - start);
		rawQuery = uri.substr(qmark + 1);
	}
	path = Encoding::urlDecode(rawPath);
	query = Encoding::urlDecode(rawQuery);
	
	if (!rawQuery.empty())
	{
		std::list<std::string> queries;
		splitString(rawQuery, "&", queries);
		for (std::list<std::string>::const_iterator it = queries.begin();
			it != queries.end();
			++it)
		{
			std::vector<std::string> parts;
			parts.reserve(2);
			splitString(*it, "=", parts);
			if (parts.size() == 1)
			{
				queryMap[Encoding::urlDecode(parts[0])] = "";
			}
			else if (parts.size() == 2)
			{
				queryMap[Encoding::urlDecode(parts[0])] =
					Encoding::urlDecode(parts[1]);
			}
			else
			{
				throw MalformedUriException("Invalid query string");
			}
		}
	}
}

unsigned short Uri::getDefaultPort() const
{
	if (protocol == "http")
		return 80;
	else if (protocol == "https")
		return 443;
	else if (protocol == "ftp")
		return 21;
	return 0;
}

unsigned short Uri::getPortFromString(const std::string & str)
{
	int port = atoi(str.c_str());
	if (!port)
	{
		throw MalformedUriException("Port either 0 or non-numeric");
	}
	else if ((port < 0) || (port > 0xffff))
	{
		throw MalformedUriException("Port either negative or too big");
	}
	return static_cast<unsigned short>(port & 0xffff);
}
