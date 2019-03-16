#include "tests_common.hpp"
#include "request.hpp"


TEST(SERVER_SUITE, parser_simple) 
{
	const std::string body = "binary body:jhabjkcnm kjOFKVMSD ZVNSLD ZVBZK NAKVJKZJDN MZ KJLC" HTTP_LINE;
	const std::string URI = "/tutorials/other/top-20-mysql-best-practices/";
	std::stringstream ss;
	ss << "GET " << URI << " HTTP/1.1" << HTTP_LINE;
	ss << "Host: net.tutsplus.com" << HTTP_LINE;
	ss << "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.5) Gecko/20091102 Firefox/3.5.5 (.NET CLR 3.5.30729)" << HTTP_LINE;
	ss << "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8" << HTTP_LINE;
	ss << "Accept-Language: en-us,en;q=0.5" << HTTP_LINE;
	ss << "Accept-Encoding: gzip,deflate" << HTTP_LINE;
	ss << "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7" << HTTP_LINE;
	ss << "Keep-Alive: 300" << HTTP_LINE;
	ss << "Connection: keep-alive" << HTTP_LINE;
	ss << "Cookie: PHPSESSID=r2t5uvjq435r4q7ib3vtdjq120" << HTTP_LINE;
	ss << "Pragma: no-cache" << HTTP_LINE;
	ss << "Cache-Control: no-cache" << HTTP_LINE;
	ss << "Content-Length: 350" << HTTP_LINE;
	ss << HTTP_LINE;
	ss << HTTP_LINE;
	ss << body;
	std::string s = ss.str();
	std::string_view sw(s);

	using namespace server;
	HTTPHeader header;
	HTTPParser(sw).Parse(header);

	EXPECT_EQ(header.method	, "GET"	);
	EXPECT_EQ(header.url	, URI	);
	EXPECT_EQ(header.body	, body	);
	EXPECT_EQ(header.conentLenght, "350");
}
