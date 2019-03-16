#ifndef REQUEST_HPP
#define REQUEST_HPP
#pragma once

#include "iserver.hpp"
#include "server_common.hpp"
#include <string_view>


namespace server
{
	struct HTTPHeader
	{
		std::string_view url;
		std::string_view method;
		std::string_view conentLenght;
		std::string_view body;
	};


	class HTTPParser
	{
		enum EParsePhase
		{
			  eMethod
			, eURI
			, eVersion
			, eKey
			, eValue
			, eEmptyLine
		};
	public:
		HTTPParser(std::string_view ss);

		void Parse(HTTPHeader& header);

	private:
		std::string_view NextToken();

	private:
		bool bValue;
		EParsePhase phase;

		std::string_view::const_iterator pos;
		std::string_view::const_iterator end;
		std::string_view ss;
	};


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
