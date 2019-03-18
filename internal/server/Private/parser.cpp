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
			{	/// empty line
				if (phase == eEmptyLine)
				{	/// second line -> body
					if (pos < end && ++pos < end)
					{
						auto data = &(*pos);
						size_t len = std::distance(pos, end);
						header.body = { data, len };
					}
					return header;
				}
				else
				{	/// first line
					phase = eEmptyLine;
				}
				continue;
			}
			switch (phase) {
			case eMethod:	phase = eURI;		header.method = token;	continue;
			case eURI:		phase = eVersion;	header.url = token;		continue;
			case eVersion:	phase = eKey;								continue;
			case eKey:		phase = eValue;		key = token;			continue;
			case eValue:	phase = eKey;
				if (key == "Content-Length")
				{
					header.conentLenght = token;
				}
				continue;
			default: return HTTPHeader();
			}
		}
	}

	std::string_view HTTPParser::NextToken()
	{
		static const auto isToken = [](const auto & begin, const auto & end)->bool
		{
			return begin != end;
		};

		auto begin = pos;
		auto bLine = false;
		for (; pos != end; pos++)
		{
			switch (*pos) {
			case ' ': case ':':
				if (bValue)
				{
					break;
				}
			case '\r': case '\n':
				bValue = *pos == ':';
				if (isToken(begin, pos))
				{	/// a token was found
					auto data = &(*begin);
					size_t len = std::distance(begin, pos);
					if (bValue)
					{	/// skip ': '
						pos += 2;
					}
					return { data, len };
				}
				else if (bLine && *pos == '\n')
				{	/// an empty line detected
					return { nullptr, 0 };
				}
				else
				{	/// second separatoin symbol
					bLine |= *pos == '\n';
					begin = pos + 1;
					break;
				}
			}
		}
		return { nullptr, 0 };
	}
} //!server
