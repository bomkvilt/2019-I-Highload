#ifndef SERVER_COMMON_HPP
#define SERVER_COMMON_HPP
#pragma once

#include "iserver.hpp"

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

namespace server
{
	namespace methods
	{
		const std::string GET = "GET";
		const std::string POST = "POST";
		const std::string HEAD = "HEAD";
	}

	namespace asio    = boost::asio;
	using SSTcp       = asio::ip::tcp;
	using SSAcceptor  = SSTcp::acceptor;
	using SSIoService = asio::io_service;
	using SSThreads   = boost::thread_group;
}


#endif //!SERVER_COMMON_HPP
