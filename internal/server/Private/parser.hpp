#ifndef PARSER_HPP
#define PARSER_HPP

#include "server_common.hpp"


namespace server
{
	struct HTTPHeader
	{
		std::string_view url;
		std::string_view query;
		std::string_view method;
		std::string_view body;
	private:
		friend class HTTPParser;
		std::string decoded;
	};


	class HTTPParser final
	{
		enum EParsePhase
		{
			  eMethod
			, eURI
			, eProto
			, eKey
			, eValue
		};
	public:
		HTTPParser(std::string_view ss);

		HTTPHeader Parse();

	private:
		std::string_view NextToken();

		void             ParseURL (HTTPHeader& header, std::string_view token);
		std::string_view DecodeURL(HTTPHeader& header, std::string_view token);

	private:
		bool bValue;
		EParsePhase phase;

		std::string_view::const_iterator pos;
		std::string_view::const_iterator end;
		std::string_view ss;
	};
}

#endif //!PARSER_HPP
