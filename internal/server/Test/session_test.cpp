#include "tests_common.hpp"
#include "session.hpp"


TEST(SERVER_SUITE, session_simple)
{
	auto io = server::SSIoService();
	auto config = server::FServerConfig();
	auto session = server::CSession(io, config);
	session.Start();
	ASSERT_FALSE("nonrealised!!!");
}
