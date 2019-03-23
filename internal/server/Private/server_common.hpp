#ifndef SERVER_COMMON_HPP
#define SERVER_COMMON_HPP
#pragma once

#include <filesystem>
#include <string_view>

#include "iserver.hpp"

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>

namespace server
{
	namespace methods
	{
		const std::string GET = "GET";
		const std::string POST = "POST";
		const std::string HEAD = "HEAD";
	}

	namespace fs	  = std::filesystem;
	namespace asio    = boost::asio;
	using SSTcp       = asio::ip::tcp;
	using SSAcceptor  = SSTcp::acceptor;
	using SSIoService = asio::io_service;
	using SSThreads   = boost::thread_group;
}


#endif //!SERVER_COMMON_HPP
