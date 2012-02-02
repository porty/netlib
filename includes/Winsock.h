#pragma once

#ifdef _WIN32

#pragma comment (lib, "ws2_32.lib")

namespace Net
{
	/**
	 * Used to automatically initialise and clean-up Winsock.
	 *
	 * @author Shorty
	 */
	class Winsock
	{
	public:
		/**
		 * Initialise Winsock.
		 */
		Winsock();

		/**
		 * Clean up winsock
		 */
		~Winsock();
	};
}

#endif
