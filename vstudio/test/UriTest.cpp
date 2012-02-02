#include "testing.h"

#include "../../includes/Uri.h"

using namespace Testing;
using namespace Net;

void testUri()
{
	{
		Uri uri("http://bill:pass@roflware.com:8080/path%20to/file.php?apple=banana&car%20rot=dingo");
		assertEqual("Incorrect protocol", uri.getProtocol(), std::string("http"));
		assertEqual("Incorrect user info", uri.getUserInfo(), std::string("bill:pass"));
		assertEqual("Incorrect hostname", uri.getHost(), std::string("roflware.com"));
		assertEqual("Incorrect port", uri.getPort(), static_cast<unsigned short>(8080));
		assertEqual("Incorrect raw path", uri.getRawPath(), std::string("/path%20to/file.php"));
		assertEqual("Incorrect path", uri.getPath(), std::string("/path to/file.php"));
		assertEqual("Incorrect raw query", uri.getRawQuery(), std::string("apple=banana&car%20rot=dingo"));
		assertEqual("Incorrect query", uri.getQuery(), std::string("apple=banana&car rot=dingo"));
		assertEqual("Incorrect number of query entries", uri.getQueryMap().size(), 2U);
		assertEqual("Incorrect query map entry", uri.getQueryMap().at("apple"), std::string("banana"));
		assertEqual("Incorrect query map entry", uri.getQueryMap().at("car rot"), std::string("dingo"));
	}
	{
		Uri uri("/path%20to/file.php?apple=banana&car%20rot=dingo");
		assertTrue("Protocol should be empty", uri.getProtocol().empty());
	}
	{
		Uri uri("mailto:man@hat.com");
		assertEqual("Incorrect protocol", uri.getProtocol(), std::string("mailto"));
	}
	{
		Uri uri("http:");
		assertEqual("Incorrect protocol", uri.getProtocol(), std::string("http"));
	}
	{
		Uri uri("http://roflware.com/path/");
		assertEqual("Incorrect hostname", uri.getHost(), std::string("roflware.com"));
		assertEqual("Incorrect port", uri.getPort(), static_cast<unsigned short>(80));
	}
	{
		Uri uri("http://roflware.com:12345/path/");
		assertEqual("Incorrect hostname", uri.getHost(), std::string("roflware.com"));
		assertEqual("Incorrect port", uri.getPort(), static_cast<unsigned short>(12345));
	}
	{
		try
		{
			Uri uri("http://roflware.com:-6/path/");
			fail("Invalid port should throw error");
		}
		catch (const MalformedUriException &)
		{
			// expected
		}
	}
	std::cout << "testUri OK" << "\n";
}
