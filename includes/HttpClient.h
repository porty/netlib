#pragma once

#include <string>
#include <ostream>
//#include <list>
#include <map>

namespace Net
{
	class PostData
	{
	public:
		void addText(const std::string & name, const std::string & value);

		void addFile(const std::string & name, const std::string & fileName);

		void generateMultipart(std::ostream & stream, const std::string & multipartBoundary);
	private:
		std::map<std::string, std::string> textItems;
		std::map<std::string, std::string> fileItems;
	};

	class HttpClient
	{
	public:
		static void postMultipart(const std::string & url, const PostData & postData);
	};
}
