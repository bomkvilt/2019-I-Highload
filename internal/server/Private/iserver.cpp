#include "iserver.hpp"
#include "server_common.hpp"
#include "session.hpp"
#include <fstream>


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
			if (!error)
			{
				session->Start();
			}
			StartAccept();
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
	void FServerConfig::LoadFromFile(const std::string& file)
	{
		FConfig::LoadFromFile(file);
		if (mainConfig == "")
		{
			return;
		}

		auto config = std::ifstream(mainConfig, std::ios::in);
		if (!config.is_open())
		{
			throw std::runtime_error("Cannot open mainFile");
		}
			
		auto line = std::string();
		while (std::getline(config, line))
		{
			auto ss = std::stringstream(line);
			auto key = std::string(); ss >> key;
			
			if (key.front() == '#')
			{
				continue;
			}
			if (key == "cpu_limit")
			{
				ss >> threads;
			}
			else if (key == "document_root")
			{
				ss >> fileDirectory;
			}
		}
	}


	UNIQUE(IServer) IServer::Create(const FServerConfig& config)
	{
		return std::make_unique<IServerImpl::FServer>(config);
	}
} //!server
