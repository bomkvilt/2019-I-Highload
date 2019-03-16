#ifndef PARSER_HPP
#define PARSER_HPP

#include "server_common.hpp"


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

		HTTPHeader Parse();

	private:
		std::string_view NextToken();

	private:
		bool bValue;
		EParsePhase phase;

		std::string_view::const_iterator pos;
		std::string_view::const_iterator end;
		std::string_view ss;
	};
}

#endif //!PARSER_HPP
