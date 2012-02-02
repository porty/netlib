#pragma once

#include <string>
#include <map>

namespace Net
{
	class MalformedUriException: public std::exception
	{
	public:
		MalformedUriException(const char * const message): std::exception(message) { }
	};

	class Uri
	{
	public:
		typedef std::map<std::string, std::string> queryMap_t;

		Uri(const std::string & uri);
		~Uri() {}

		const std::string & getHost() const			{ return host; }
		unsigned short getPort() const				{ return port; }
		const std::string & getProtocol() const		{ return protocol; }
		const std::string & getRawPath() const		{ return rawPath; }
		const std::string & getPath() const			{ return path; }
		const std::string & getRawQuery() const		{ return rawQuery; }
		const std::string & getQuery() const		{ return query; }
		const std::string & getUserInfo() const		{ return userInfo; }
		const queryMap_t & getQueryMap() const		{ return queryMap; }
	private:
		std::string host;
		unsigned short port;
		std::string protocol;
		std::string rawPath;
		std::string path;
		std::string rawQuery;
		std::string query;
		std::string userInfo;
		queryMap_t queryMap;
	private:
		std::string::size_type i;
		const std::string & uri;
		void doProtocol();
		void doUserHostAndPort();
		void doPath();
		unsigned short getDefaultPort() const;
		static unsigned short getPortFromString(const std::string & str);
	private:
		// do not implement
		Uri & operator=(const Uri &);
	};
}
