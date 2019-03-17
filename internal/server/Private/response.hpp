#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <sstream>
#include <unordered_map>
#include "server_common.hpp"


namespace server
{
	struct FResponse
	{
		using SSHeaders = std::unordered_map<std::string, std::string>;

	public:
		int			code		= 200;
		std::string description	= "OK";
		SSHeaders	headers;
		std::string body;

	public:
		FResponse& SetCode(int newCode);
		FResponse& SetDescription(std::string newDescription);
		FResponse& SetHeader(const std::string& key, std::string value);
		FResponse& SetBody(std::string newBody);

		std::string ToString() const
		{
			static const auto LINE = "\r\n";
			std::stringstream ss;
			ss << "HTTP/1.1 " << code << " " << description << LINE;
			for (auto&& [key, value] : headers)
			{
				ss << key << ": " << value << LINE;
			}
			ss << LINE;
			ss << LINE;
			ss << body;
			return ss.str();
		}

	public:
		const static FResponse C200;
		const static FResponse C403;
		const static FResponse C404;
		const static FResponse C405;
	};
}

#endif //!RESPONSE_HPP
