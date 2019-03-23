#include "response.hpp"

namespace server
{
	FResponse& FResponse::SetCode(int newCode)
	{
		code = newCode;
		return *this;
	}

	FResponse& FResponse::SetStatus(std::string newDescription)
	{
		status = std::move(newDescription);
		return *this;
	}

	FResponse& FResponse::SetHeader(const std::string& key, std::string value)
	{
		headers[key] = std::move(value);
		return *this;
	}
	
	FResponse& FResponse::SetBody(std::string newBody)
	{ 
		body = (body_data = std::move(newBody));
		return SetHeader("Content-Length", std::to_string(body.length()));
	}

	FResponse& FResponse::SetBody(std::string_view newBody)
	{
		body = newBody;
		return SetHeader("Content-Length", std::to_string(body.length()));
	}

	std::string FResponse::ToString() const
	{
		static const auto LINE = "\r\n";
		std::stringstream ss;
		ss << "HTTP/1.1 " << code << " " << status << LINE;
		for (auto&& [key, value] : headers)
		{
			ss << key << ": " << value << LINE;
		}
		ss << LINE;
		ss << body;
		return ss.str();
	}


	const FResponse FResponse::C200 = FResponse { 200, "OK"					, {}, "", "" };
	const FResponse FResponse::C403 = FResponse { 403, "Forbidden"			, {}, "", "" };
	const FResponse FResponse::C404 = FResponse { 404, "Not Found"			, {}, "", "" };
	const FResponse FResponse::C405 = FResponse { 405, "Method Not Allowed"	, {}, "", "" };
}
