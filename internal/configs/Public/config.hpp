#ifndef CONFIG_HPP
#define CONFIG_HPP
#pragma once

#include <string>
#include "Archived.hpp"


struct FConfig : public Archived
{
	GENERATED_BODY(FConfig, Archived);
public:
	void LoadFromFile(const std::string& file);
	void SafeToFile(const std::string& file);
};


#endif //!CONFIG_HPP
