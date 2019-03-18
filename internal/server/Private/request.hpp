#ifndef REQUEST_HPP
#define REQUEST_HPP
#pragma once

#include "parser.hpp"
#include "response.hpp"


namespace server
{
	struct FRequest
	{
	public:
		FRequest(std::string_view request, FServerConfig config);

		FResponse Parse();

	private:
		FResponse OnGet();
		FResponse OnHead();
		FResponse OnUnknownMethed();

		std::tuple<std::string, bool> GetPath(std::string_view relativePath);

	private:
		std::string header_url; //!< RAII for updated @header.url
		std::string_view request;
		FServerConfig config;
		HTTPHeader header;
	};
}


#endif //!REQUEST_HPP
