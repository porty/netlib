#include "HttpClient.h"
#include "FileSystem.h"

#include <iostream>
#include <fstream>
#include <exception>

using namespace Net;

void PostData::addText(const std::string & name, const std::string & value)
{
	textItems[name] = value;
}

void PostData::addFile(const std::string & name, const std::string & fileName)
{
	if (!OS::FileSystem::fileExists(fileName))
	{
		const std::string errText = "Specified file " + fileName + " doesn't exist";
		throw std::exception(errText.c_str());
	}
	fileItems[name] = fileName;
}

void PostData::generateMultipart(std::ostream & stream, const std::string & multipartBoundary)
{
	// for each text item
	for (std::map<std::string, std::string>::const_iterator it = textItems.begin(); it != textItems.end(); ++it)
	{
		stream << multipartBoundary << "\r\n"
			"Content-Disposition: form-data; name=\"" << it->first << "\"\r\n"
			"Content-Type: text/plain\r\n"
			"\r\n"
			<< it->second << "\r\n";
	}

	// for each file item
	for (std::map<std::string, std::string>::const_iterator it = fileItems.begin(); it != fileItems.end(); ++it)
	{
		stream << multipartBoundary << "\r\n"
			"Content-Disposition: form-data; name=\"" << it->first << "\" ; filename=\"" << OS::FileSystem::nameOnly(it->second) << "\"\r\n"
			"Content-Type: application/octet-stream\r\n"
			"\r\n";
		
		std::fstream file(it->second.c_str(), std::ios_base::binary | std::ios_base::in);
		if (!file.is_open())
		{
			std::string errorMessage = "Failed to open file " + it->second + " for reading";
			throw std::exception(errorMessage.c_str());
		}

		{
			char buf[4096];
			std::streamsize readSize;
			while ((readSize = file.readsome(buf, sizeof(buf))) > 0)
			{
				stream.write(buf, readSize);
			}
		}

		stream << "\r\n";
	}

	stream << multipartBoundary << "\r\n";
}
