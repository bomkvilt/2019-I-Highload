#include "request.hpp"

namespace server 
{
	void FRequest::Parse(std::function<void(std::string_view)> callback)
	{
		header = HTTPParser(request).Parse();
	}
} //!server
