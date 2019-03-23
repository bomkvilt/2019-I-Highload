#include "request.hpp"
#include "date/date.h"
#include "mio/mmap.hpp"

#include <fstream>
#include <sstream>
#include <chrono>
#include <shared_mutex>
#include <mutex>


namespace server::FRequestUtil
{
	struct FFileMapper
	{
	public:
		std::tuple<std::string_view, bool> ReadFile(const std::string& s)
		{	// try 2 find an existing file
			if (std::shared_lock _(mutex); _)
			{
				if (auto [sv, ok] = Try2Find(s); ok)
				{
					return { sv, ok };
				}
			}

			// try 2 load a new file
			std::unique_lock _(mutex);
			
			if (auto [sv, ok] = Try2Find(s); ok)
			{
				return { sv, ok };
			}

			auto code = std::error_code();
			auto file = mio::make_mmap_source(s, code);
			if (code)
			{
				return { { nullptr, 0 }, false };
			}

			files.emplace(s, std::move(file));
			return Try2Find(s);
		}

	private:
		std::unordered_map<std::string, mio::mmap_source> files;
		std::shared_mutex mutex;

		std::tuple<std::string_view, bool> Try2Find(const std::string& s)
		{
			auto pos = files.find(s);
			auto end = files.end();
			if (pos != end)
			{
				auto&& [name, map] = *pos;
				return { { map.data(), map.size() }, true };
			}
			return { { nullptr, 0 }, false };
		}
	};

	static FFileMapper maper;
}


namespace server 
{
	FRequest::FRequest(std::string_view request, FServerConfig config)
		: request(request)
		, config(config)
	{
		header = HTTPParser(request).Parse();
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
					auto t = date::floor<date::days>(p);
					auto ss = std::stringstream();
					return ss << date::year_month_day(t), ss.str();
				}());
	}

	FResponse FRequest::OnGet()
	{
		if (auto [path, state] = GetPath(header.url); state)
		{
			if (auto [file, ok] = FRequestUtil::maper.ReadFile(path.string()); !ok)
			{
				return  state == eFile
					? FResponse::C404
					: FResponse::C403;
			}
			else return FResponse(FResponse::C200).SetBody(file);
		}
		else return FResponse::C403;
	}
	
	FResponse FRequest::OnHead()
	{
		if (auto [path, state] = GetPath(header.url); state)
		{
			if (auto [file, ok] = FRequestUtil::maper.ReadFile(path.string()); !ok)
			{
				return  state == eFile
					? FResponse::C404
					: FResponse::C403;
			}
			else return FResponse(FResponse::C200).SetHeader("Content-Length", std::to_string(file.size()));
		}
		else return FResponse::C403;
	}
	
	FResponse FRequest::OnUnknownMethed()
	{
		return FResponse::C405;
	}
	
	std::tuple<fs::path, FRequest::EPathType> FRequest::GetPath(std::string_view relativePath)
	{
		auto root = fs::path(config.fileDirectory);
		auto path = fs::path(config.fileDirectory + std::string(relativePath));
		for (auto&& dir : fs::relative(path, root))
		{
			if (dir == "..")
			{
				return { fs::path(), eForbidden };
			}
		}
		if (fs::is_directory(path))
		{
			return { path / config.defaultURI, eDirectory };
		}
		return { path, eFile };
	}
} //!server
