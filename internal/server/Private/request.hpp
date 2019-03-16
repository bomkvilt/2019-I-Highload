#ifndef REQUEST_HPP
#define REQUEST_HPP
#pragma once

#include "parser.hpp"
#include <string_view>


namespace server
{
	struct FRequest
	{
		FRequest(std::string_view request, FServerConfig config)
			: request(request)
			, config(config)
		{}

		void Parse(std::function<void(std::string_view)> callback);

	private:
		std::string_view request;
		FServerConfig config;
		HTTPHeader header;
	};
}


#endif //!REQUEST_HPP
