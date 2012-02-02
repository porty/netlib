#include <exception>
#include <iostream>

#include "UriTest.h"

int main()
{
	try
	{
		testUri();
	}
	catch (const std::exception & ex)
	{
		std::cerr << ex.what() << std::endl;
		return -1;
	}
}
