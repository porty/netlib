#pragma once

#include <string>
#include <exception>

namespace Net
{
	class Encoding
	{
	public:
		class EncodingException: public std::exception
		{
		public:
			EncodingException(const char * const message): std::exception(message) {}
		};

		static std::string urlEncode(const std::string in);
		static std::string urlDecode(const std::string in);
	private:
		Encoding();
		Encoding(const Encoding &);
		Encoding operator=(const Encoding &);
	};
}
