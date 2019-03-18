#ifndef SESSION_HPP
#define SESSION_HPP
#pragma once

#include <array>

#include "server_common.hpp"
#include "request.hpp"

#include <boost/enable_shared_from_this.hpp>


namespace server
{
	class CSession final : public std::enable_shared_from_this<CSession>
	{
	public:
		SSTcp::socket& GetSocket()
		{
			return socket;
		}

		CSession(SSIoService& service, FServerConfig config);

		~CSession();
		
		void Start();

		void Stop();

	private:
		void HandleRead (const boost::system::error_code& error, size_t size);
		void HandleWrite(const boost::system::error_code& error, size_t size);
		void WriteString(std::string_view string);

	private:
		FServerConfig config;
		SSTcp::socket socket;

		enum {
			filePartSize = 16384
			, bufferSize = 1024
		};
		std::array<char, bufferSize> buffer;

		ptrdiff_t offset;
	};
}


#endif //!SESSION_HPP
