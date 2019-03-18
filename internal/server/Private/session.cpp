#include "session.hpp"


namespace server
{
	CSession::CSession(SSIoService& service, FServerConfig config)
		: socket(service)
		, config(config)
		, offset(0)
	{}

	CSession::~CSession()
	{
		Stop();
	}

	void CSession::Start()
	{
		socket.non_blocking(true);
		socket.native_non_blocking(true);
		socket.async_read_some(
			asio::buffer(buffer),
			boost::bind(&CSession::HandleRead, shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred)
		);
	}

	void CSession::Stop()
	{
		if (socket.is_open())
		{
			socket.close();
		}
	}

	void CSession::HandleRead(const boost::system::error_code& error, size_t size)
	{
		if (error)
		{
			Stop();
			return;
		}
		auto request = FRequest(std::string_view{ buffer.data(), size }, config);
		WriteString(request.Parse().ToString());
	}

	void CSession::HandleWrite(const boost::system::error_code& error, size_t size)
	{
		if (error)
		{
			Stop();
			return;
		}
	}

	void CSession::WriteString(std::string_view string)
	{
		socket.async_write_some(
			asio::buffer(string),
			boost::bind(&CSession::HandleWrite, shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred)
		);
	}
}
