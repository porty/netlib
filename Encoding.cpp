#include "Encoding.h"

using namespace Net;

std::string Encoding::urlEncode(const std::string in)
{
	std::string out;
	out.reserve(in.size() + 1); // because we are going to need this much at least
	char urlcodedstub[4] = "%xx";
	for(unsigned long i = 0; i < in.size(); i++)
	{
		// test for alphabetical, numerical, or in "$-_.+!*'(),"
		if (
				// alphabetical
				((('a' <= in[i]) && (in[i] <= 'z')) || (('A' <= in[i]) && (in[i] <= 'Z'))) ||
				// numerical
				(('0' <= in[i]) && (in[i] <= '9')) ||
				// other
				(in[i] == '$') ||
				(in[i] == '-') ||
				(in[i] == '_') ||
				(in[i] == '.') ||
				(in[i] == '+') ||
				(in[i] == '!') ||
				(in[i] == '*') ||
				(in[i] == '\'') ||
				(in[i] == '(') ||
				(in[i] == ')') ||
				(in[i] == ',')
			)
		{
			// was just a-z or A-Z
			out.push_back(in[i]);
		}
		else
		{
			// urlcodedstub+1 for overwriting the two chars but not the %
			if((unsigned char) in[i] < 10)
			{
				urlcodedstub[1] = '0';
				_itoa_s(in[i], urlcodedstub + 2, 2, 16);
			}
			else
			{
				_itoa_s(in[i], urlcodedstub + 1, 3, 16);
			}
			out += urlcodedstub;
		}
	}
	return out;
}

std::string Encoding::urlDecode(const std::string in)
{
	std::string out;
	out.reserve(in.size()); // because we are going to need this much at most
	for(unsigned long i = 0; i < in.size(); i++)
	{
		if(in[i] == '%')
		{
			if(i + 2 >= in.size())
			{
				throw new EncodingException("Invalid URL encoded string: premature end of string");
			}
			//char t[] = {Output[i+1], Output[i+2], 0};
			char t[] = { in[i+1], in[i+2], 0 };
			char * ok;
			long r = strtol((char *)t, &ok, 16);
			//if(!ok)
			if(!ok || (ok == t))
			{
				throw new EncodingException("Invalid URL encoded string: invalid encoded value");
			}
			out.push_back((char)r);
			i += 2;
		}
		else
		{
			out.push_back(in[i]);
		}
	}
	return out;
}
