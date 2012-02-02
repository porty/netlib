#pragma once

#include "UnionCast.h"
#include "macros.h"

#include <stdint.h>
#include <string>

namespace Net
{
	/**
	 * Class to help with IP address stuffs.
	 *
	 * @author Shorty
	 */
	class IPAddress
	{
	public:
		/**
		 * Create a new IP address, given a 32-bit IP address number.
		 *
		 * @param address The IP address to use.
		 */
		IPAddress(uint32_t address): address(address) {}

		/**
		 * Create a new IP address, given the parts of a dotted quad.
		 *
		 * @param one The first dotted quad
		 * @param two The second dotted quad
		 * @param three The third dotted quad
		 * @param four The fourth dotted quad
		 */
		IPAddress(uint8_t one, uint8_t two, uint8_t three, uint8_t four):
			address(fromQuad(one, two, three, four)) {}

		/**
		 * Get the string representation of this IP address.
		 *
		 * @return The string representation of this IP address in dotted quad.
		 */
		std::string toString() const;

		/**
		 * Get the 32-bit integer of this IP address.
		 *
		 * @return The 32-bit integer representation of this IP address.
		 */
		uint32_t getAddress() const { return address; }
	private: // static
		/**
		 * Create a 32-bit number from four 8-bit ones.
		 */
		static uint32_t fromQuad(uint8_t one, uint8_t two, uint8_t three,
			uint8_t four);
	private: // non-static
		/** The IP address. */
		const uint32_t address;

	private:
		//NO_COPY_CONSTRUCTOR(IPAddress);
		NO_ASSIGNMENT_OPERATOR(IPAddress);
	};
}
