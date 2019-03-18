#include "tests_common.hpp"
#include "request.hpp"
#include <fstream>


struct server_request_tests : testing::Test
{
	void SetUp() override
	{
		config.fileDirectory = ".";
		config.defaultURI = "main.txt";

		auto test_txt = std::ofstream("test.txt", std::ios::out);
		assert(test_txt.is_open()); test_txt << test_txt_text; test_txt.close();

		auto main_txt = std::ofstream("main.txt", std::ios::out);
		assert(main_txt.is_open()); main_txt << main_txt_text; main_txt.close();
	}

	void TearDown() override
	{
		server::fs::remove(server::fs::path("test.txt"));
		server::fs::remove(server::fs::path("main.txt"));
	}

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

	const std::string test_txt_text = "aa";
	const std::string main_txt_text = "bb";
};


TEST_F(server_request_tests, request_simple_get)
{	// inallowed method
	auto response = GetResponse("/test.txt", "GET");
	EXPECT_EQ(response.code, 200);
	EXPECT_EQ(response.body, test_txt_text);
}

TEST_F(server_request_tests, request_simple_head)
{	// inallowed method
	auto response = GetResponse("/test.txt", "HEAD");
	EXPECT_EQ(response.code, 200);
	EXPECT_EQ(response.body, "");
}

TEST_F(server_request_tests, request_default_path)
{	// inallowed method
	auto response = GetResponse("/");
	EXPECT_EQ(response.code, 200);
	EXPECT_EQ(response.body, main_txt_text);
}

TEST_F(server_request_tests, request_inallowed_method)
{	// inallowed method
	auto response = GetResponse("/../test.txt", "POST");
	EXPECT_EQ(response.code, 405);
}

TEST_F(server_request_tests, request_out_of_direcory_1)
{	// path out of directory
	auto response = GetResponse("/../test.txt");
	EXPECT_EQ(response.code, 403);
}

TEST_F(server_request_tests, request_out_of_direcory_2)
{	// path out of directory
	auto response = GetResponse("/main/../../test.txt");
	EXPECT_EQ(response.code, 403);
}

TEST_F(server_request_tests, request_out_of_direcory_3)
{	// .. must be allowed
	auto response = GetResponse("/main/../test.txt");
	EXPECT_EQ(response.code, 200);
}
