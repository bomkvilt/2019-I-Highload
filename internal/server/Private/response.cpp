#include "response.hpp"

namespace server
{
	FResponse& FResponse::SetCode(int newCode)
	{
		code = newCode;
		return *this;
	}

	FResponse& FResponse::SetDescription(std::string newDescription)
	{
		description = std::move(newDescription);
		return *this;
	}

	FResponse& FResponse::SetHeader(const std::string& key, std::string value)
	{
		headers[key] = std::move(value);
		return *this;
	}
	
	FResponse& FResponse::SetBody(std::string newBody)
	{
		body = std::move(newBody);
		return SetHeader("Content-Length", std::to_string(body.length()));
	}


	const FResponse FResponse::C200 = FResponse
	{
		  200
		, "OK"
		, {}
		, ""
	};
	const FResponse FResponse::C403 = FResponse
	{
		  403
		, "Forbidden"
		, {}
		, ""
	};
	const FResponse FResponse::C404 = FResponse
	{
		  404
		, "Not Found"
		, {}
		, ""
	};
	const FResponse FResponse::C405 = FResponse
	{
		  405
		, "Method Not Allowed"
		, {}
		, ""
	};
}
