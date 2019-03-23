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
		enum EPathType
		{
			  eForbidden	//!<
			, eFile			//!< 
			, eDirectory	//!< 
		};

	public:
		FRequest(std::string_view request, FServerConfig config);

		FResponse Parse();

	private:
		FResponse OnGet();
		FResponse OnHead();
		FResponse OnUnknownMethed();

		std::tuple<fs::path, EPathType> GetPath(std::string_view relativePath);

	private:
		std::string_view request;
		FServerConfig config;
		HTTPHeader header;
	};
}


#endif //!REQUEST_HPP
