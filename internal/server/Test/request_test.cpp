#include "tests_common.hpp"
#include "request.hpp"
#include <fstream>


struct server_request_tests : testing::Test
{
	void SetUp() override
	{
		config.fileDirectory = ".";
		config.defaultURI = "__request_main.txt";
	}

	void TearDown() override
	{}

	static struct A
	{
		A()
		{
			auto test_txt = std::ofstream("__request_test.txt", std::ios::out);
			assert(test_txt.is_open()); test_txt << test_txt_text; test_txt.close();

			auto main_txt = std::ofstream("__request_main.txt", std::ios::out);
			assert(main_txt.is_open()); main_txt << main_txt_text; main_txt.close();
		}
	} a;

	std::string ConstructRequest(std::string_view method, std::string_view url)
	{
		std::stringstream ss;
		ss << method << " " << url <<" HTTP/1.1" << HTTP_LINE;
		ss << HTTP_LINE;
		ss << HTTP_LINE;
		return ss.str();
	}

	server::FResponse GetResponse(std::string_view url, std::string_view method = "GET")
	{
		return server::FRequest(ConstructRequest(method, url), config).Parse();
	}

public:

	server::FServerConfig config;

	constexpr static std::string_view test_txt_text = "aa";
	constexpr static std::string_view main_txt_text = "bb";
};

server_request_tests::A server_request_tests::a;


TEST_F(server_request_tests, simple_get)
{	// inallowed method
	auto response = GetResponse("/__request_test.txt", "GET");
	EXPECT_EQ(response.code, 200);
	EXPECT_EQ(response.body, test_txt_text);
}

TEST_F(server_request_tests, simple_head)
{	// inallowed method
	auto response = GetResponse("/__request_test.txt", "HEAD");
	EXPECT_EQ(response.code, 200);
	EXPECT_EQ(response.body, "");
}

TEST_F(server_request_tests, default_path)
{	// inallowed method
	auto response = GetResponse("/");
	EXPECT_EQ(response.code, 200);
	EXPECT_EQ(response.body, main_txt_text);
}

TEST_F(server_request_tests, inallowed_method)
{	// inallowed method
	auto response = GetResponse("/../__request_test.txt", "POST");
	EXPECT_EQ(response.code, 405);
}

TEST_F(server_request_tests, out_of_direcory_1)
{	// path out of directory
	auto response = GetResponse("/../__request_test.txt");
	EXPECT_EQ(response.code, 403);
}

TEST_F(server_request_tests, out_of_direcory_2)
{	// path out of directory
	auto response = GetResponse("/main/../../__request_test.txt");
	EXPECT_EQ(response.code, 403);
}

TEST_F(server_request_tests, out_of_direcory_3)
{	// .. must be allowed
	auto response = GetResponse("/main/../__request_test.txt");
	EXPECT_EQ(response.code, 200);
}
