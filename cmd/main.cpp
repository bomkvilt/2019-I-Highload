#include <iostream>
#include "iserver.hpp"


int main(int argsc, char** argv)
{
	auto config = server::FServerConfig();
	config.LoadFromFile("./config.json");

	auto srv = server::IServer::Create(server::FServerConfig{});
}
