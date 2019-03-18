#include "request.hpp"
#include <fstream>
#include <chrono>
#include <sstream>
#include <ctime>


namespace server 
{
	FRequest::FRequest(std::string_view request, FServerConfig config)
		: request(request)
		, config(config)
	{
		header = HTTPParser(request).Parse();
		if (*header.url.rbegin() == '/')
		{	// fix directory path
			header_url = std::string(header.url) + config.defaultURI;
			header.url = header_url;
		}
	}
	
	FResponse FRequest::Parse()
	{
		return [this]()->FResponse
		{
			if (header.method == "GET")
			{
				return OnGet();
			}
			if (header.method == "HEAD")
			{
				return OnHead();
			}
			return OnUnknownMethed();
		}()
			.SetHeader("Server", "boser-0.1.0")
			.SetHeader("Connection", "close")
			.SetHeader("Date", []()->std::string
				{
					using std::chrono::system_clock;
					auto p = system_clock::now();
					auto t = system_clock::to_time_t(p);
					return std::ctime(&t);
				}());
	}

	FResponse FRequest::OnGet()
	{
		if (auto [path, ok] = GetPath(header.url); ok)
		{
			auto file = std::ifstream(path, std::ios::in | std::ios::binary);
			if (!file.is_open())
			{
				return FResponse::C404;
			}
			auto data = std::string(std::istreambuf_iterator(file), {});
			return FResponse(FResponse::C200).SetBody(std::move(data));
		}
		else return FResponse::C403;
	}
	
	FResponse FRequest::OnHead()
	{
		if (auto [path, ok] = GetPath(header.url); ok)
		{
			auto file = std::ifstream(path, std::ios::in | std::ios::binary);
			if (!file.is_open())
			{
				return FResponse::C404;
			}
			return FResponse::C200;
		}
		else return FResponse::C403;
	}
	
	FResponse FRequest::OnUnknownMethed()
	{
		return FResponse::C405;
	}
	
	std::tuple<std::string, bool> FRequest::GetPath(std::string_view relativePath)
	{
		auto root = fs::path(config.fileDirectory);
		auto path = fs::path(config.fileDirectory + std::string(relativePath));
		for (auto&& dir : fs::relative(path, root))
		{
			if (dir == L"..")
			{
				return { "", false };
			}
		}
		return { config.fileDirectory + std::string(relativePath), true };
	}
} //!server
