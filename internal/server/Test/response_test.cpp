#include "tests_common.hpp"
#include "request.hpp"

#define SERVER_RESPONSE_TESTS server_response_tests


TEST(SERVER_RESPONSE_TESTS, headers_simple)
{
	using namespace std::literals;

	std::stringstream ss;
	ss << "HTTP/1.1 200 OK" << HTTP_LINE;
	ss << "Server: aa" << HTTP_LINE;
	ss << "Date: bb" << HTTP_LINE;
	ss << "Content-Length: 4" << HTTP_LINE;
	ss << HTTP_LINE;
	ss << "ggwp";
	auto s = ss.str();
	
	auto r = server::FResponse()
		.SetCode	(200)
		.SetStatus	("OK")
		.SetHeader	("Server", "aa")
		.SetHeader	("Date", "bb")
		.SetBody	("ggwp"sv)
		.ToString();
	EXPECT_EQ(s, r);
}
