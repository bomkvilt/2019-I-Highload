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
		int					code	= 200;
		std::string			status	= "OK";
		SSHeaders			headers	= {};
		std::string_view	body	= {};
		std::string body_data = {};

	public:
		FResponse& SetCode(int newCode);
		FResponse& SetStatus(std::string newDescription);
		FResponse& SetHeader(const std::string& key, std::string value);
		FResponse& SetBody(std::string      newBody);
		FResponse& SetBody(std::string_view newBody);

		std::string ToString() const;

	public:
		const static FResponse C200;
		const static FResponse C403;
		const static FResponse C404;
		const static FResponse C405;
	};
}

#endif //!RESPONSE_HPP
