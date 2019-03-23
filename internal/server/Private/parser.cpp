#include "parser.hpp"

namespace server
{
	HTTPParser::HTTPParser(std::string_view ss)
		: ss(ss)
		, bValue(false)
	{
		phase = eMethod;
		pos = ss.begin();
		end = ss.end();
	}

	HTTPHeader HTTPParser::Parse()
	{
		HTTPHeader header;
		std::string_view key;
		for (;;)
		{
			auto token = NextToken();
			if (token.size() == 0)
			{	//!^ empty line
				if (pos < end && ++pos < end)
				{	// if body exists
					auto data = &(*pos);
					size_t len = std::distance(pos, end);
					header.body = { data, len };
				}
				return header;
			}
			switch (phase) {
			case eMethod	:	phase = eURI	; header.method = token;	continue;
			case eURI		:	phase = eProto	; ParseURL(header, token);	continue;
			case eProto		:	phase = eKey	;							continue;
			case eKey		:	phase = eValue	; key = token;				continue;
			case eValue		:	phase = eKey	;							continue;
			default: return HTTPHeader();
			}
		}
	}

	std::string_view HTTPParser::NextToken()
	{
		constexpr auto isToken = [](const auto begin, const auto end)
		{	// if non-empty
			return begin != end;
		};

		auto bLine = false;
		//!^ is '\n' detected
		for (auto begin = pos; pos != end; pos++)
		{	// parse delimeters
			switch (*pos) {
			case ' ': case ':':
				// skip the delimeters for values
				if (bValue)
				{
					break;
				}
			case '\r': case '\n':
				bValue = *pos == ':';
				//!^ is a next token a header value 
				if (isToken(begin, pos))
				{	// a token has been found
					auto data = &*begin;
					auto size = std::distance(begin, pos);
					if (bValue)
					{	// escape ": "
						std::advance(pos, 2);
					}
					return { data, size_t(size) };
				}
				else if (bLine && *pos == '\n')
				{	// an empty line
					return { nullptr, 0 };
				}
				else
				{	// second separator
					bLine |= *pos == '\n';
					begin = std::next(pos);
				}
				break;
			}
		}
		return { nullptr, 0 };
	}

	void HTTPParser::ParseURL(HTTPHeader& header, std::string_view token)
	{
		token = DecodeURL(header, token);
		auto p = token.find('?');
		if (p != token.npos)
		{
			header.query = token.substr(p + 1);
		}
		header.url = token.substr(0, p);
	}

	std::string_view HTTPParser::DecodeURL(HTTPHeader& header, std::string_view token)
	{
		auto decoded = std::string();
		auto pos = token.begin();
		auto end = token.end();
		auto append = [&decoded, &pos, &token](char letter, bool bDecoded = false)
		{	
			if (token.size() > decoded.capacity() && bDecoded)
			{
				decoded.reserve(token.size());
				decoded.insert(decoded.begin(), token.begin(), std::prev(pos, 2));
			}
			if (decoded.size())
			{
				decoded.push_back(letter);
			}
		};
		
		std::array<char, 3> code;
		for (; pos < end; ++pos)
		{
			if (*pos == '%')
			{	// decode
				unsigned value;
				code[0] = *++pos;
				code[1] = *++pos;
				code[2] = '\0';
				sscanf(code.data(), "%X", &value);
				append((char)value, true);
			}
			else if (*pos == '+')
			{
				append(' ', true);
			}
			else append(*pos);
		}

		if (decoded.size())
		{	// change token source
			return header.decoded = std::move(decoded);
		}
		return token;
	}
} //!server
