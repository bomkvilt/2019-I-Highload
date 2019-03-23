#include "tests_common.hpp"
#include "server_common.hpp"
#include <fstream>


struct server_config_tests : public testing::Test
{
	void SetUp() override
	{}

	void TearDown() override
	{}

	static struct A
	{
		A() 
		{
			{
				auto of = std::ofstream("config_config_1.json", std::ios::out);
				assert(of.is_open());
				of <<
					"{"
					"     \"port\"          : \"80\""
					",    \"threads\"       : \"8\""
					",    \"fileDirectory\" : \".\""
					",    \"defaultURI\"    : \"__config.json\""
					",    \"mainConfig\"    : \"config_main.txt\""
					"}";
				of.close();
			}
			{
				auto of = std::ofstream("config_config_2.json", std::ios::out);
				assert(of.is_open());
				of << "{"
					"     \"port\"          : \"80\""
					",    \"threads\"       : \"8\""
					",    \"fileDirectory\" : \".\""
					",    \"defaultURI\"    : \"__config.json\""
					"}";
				of.close();
			}
			{
				auto of = std::ofstream("config_main.txt", std::ios::out);
				assert(of.is_open());
				of << "cpu_limit 4         # maximum CPU count to use (for non-blocking servers)" << std::endl;
				of << "document_root ./mm  # gg_wp easy" << std::endl;
				of.close();
			}
		}
	} a;
};
server_config_tests::A server_config_tests::a;


TEST_F(server_config_tests, no_main_config)
{
	auto config = server::FServerConfig();
	config.LoadFromFile("config_config_2.json");

	EXPECT_EQ(config.port, 80);
	EXPECT_EQ(config.threads, 8);
	EXPECT_EQ(config.fileDirectory, ".");
	EXPECT_EQ(config.defaultURI, "__config.json");
}

TEST_F(server_config_tests, with_main_config)
{
	auto config = server::FServerConfig();
	config.LoadFromFile("config_config_1.json");

	EXPECT_EQ(config.port, 80);
	EXPECT_EQ(config.threads, 4);
	EXPECT_EQ(config.fileDirectory, "./mm");
	EXPECT_EQ(config.defaultURI, "__config.json");
}
