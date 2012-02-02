#pragma once

#include <exception>
#include <string>
#include <iostream>
#include <sstream>

namespace Testing
{
	void fail(const std::string & message);

	void assertTrue(const std::string & message, bool actual);

	void assertFalse(const std::string & message, bool actual);

	template <typename T>
	void assertEqual(const std::string & message, const T & a, const T & b)
	{
		if (!(a == b))
		{
			std::stringstream ss;
			ss << message << ". Expected " << b << ", received " << a;
			fail(ss.str());
		}
	}
}
