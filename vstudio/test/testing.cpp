#include "testing.h"

void Testing::fail(const std::string & message)
{
	throw std::exception(message.c_str());
}

void Testing::assertTrue(const std::string & message, bool actual)
{
	if (!actual)
	{
		fail(message);
	}
}

void Testing::assertFalse(const std::string & message, bool actual)
{
	if (actual)
	{
		fail(message);
	}
}
