#include "iserver.hpp"
#include "server_common.hpp"
#include "session.hpp"


namespace server::IServerImpl 
{
	class FServer : public IServer
	{
	public:
		FServer(const FServerConfig& config)
			: acceptor(io, SSTcp::endpoint(SSTcp::v4(), config.port))
			, config(config)
		{
			acceptor.non_blocking(true);
		}

		virtual void Start() override
		{
			for (auto i = 0; i < config.threads; ++i)
			{
				threads.create_thread(boost::bind(&SSIoService::run, &io));
				io.post(boost::bind(&FServer::StartAccept, this));
			}
			threads.join_all();
		}

	private:
		void StartAccept()
		{
			auto connection = std::make_shared<CSession>(io, config);
			acceptor.async_accept(
				connection->GetSocket(),
				boost::bind(&FServer::HandleAccept, this, 
					connection, 
					boost::asio::placeholders::error)
			);
		}

		void HandleAccept(SHARED(CSession) session, boost::system::error_code error)
		{
			if (!acceptor.is_open())
			{
				return;
			}
			if (error)
			{
				StartAccept();
			}
			else
			{
				session->Start();
			}
		}

	private:
		SSIoService io;
		SSAcceptor acceptor;
		SSThreads threads;

		FServerConfig config;
	};
} //!server::IServerImpl


namespace server 
{
	UNIQUE(IServer) IServer::Create(const FServerConfig& config)
	{
		return std::make_unique<IServerImpl::FServer>(config);
	}
} //!server
