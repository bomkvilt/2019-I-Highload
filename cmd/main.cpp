#include <iostream>
#include "iserver.hpp"

int main(int argsc, char** argv)
{
	auto srv = server::IServer::Create(server::FServerConfig{});

	printf("Hello world!");
}
