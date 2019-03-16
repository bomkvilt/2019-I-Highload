#ifndef ISERVER_HPP
#define ISERVER_HPP

#include "config.hpp"


namespace server
{
	struct FServerConfig : public FConfig
	{
		GENERATED_ARCHIVATION_BODY(FServerConfig, FConfig)
			ARCHIVATION_MEMBER("", port)
			ARCHIVATION_MEMBER("", threads)
			ARCHIVATION_MEMBER("", fileDirectory)
			ARCHIVATION_END()
	public:
		unsigned short port;
		unsigned short threads;
		std::string fileDirectory;
	};


	struct IServer
	{
		static UNIQUE(IServer) Create(const FServerConfig& config);
	public:

		virtual void Start() = 0;

		~IServer() = default;
	};
}


#endif //!ISERVER_HPP
