#include "IPAddress.h"

using namespace Net;

std::string IPAddress::toString() const
{
	// TODO
	return "";
}

uint32_t IPAddress::fromQuad(uint8_t one, uint8_t two, uint8_t three, uint8_t four)
{
	union
	{
		struct
		{
			uint8_t four;
			uint8_t three;
			uint8_t two;
			uint8_t one;
		};
		uint32_t big;
	} ipUnion;

	ipUnion.one = one;
	ipUnion.two = two;
	ipUnion.three = three;
	ipUnion.four = four;
	return ipUnion.big;
}
