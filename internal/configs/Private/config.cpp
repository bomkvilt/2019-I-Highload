#include "config.hpp"
#include <fstream>

namespace FConfigUtil
{
	template<class FS>
	inline void CheckFile(const FS& fs, const std::string& file)
	{
		if (!fs.is_open())
		{
			throw std::runtime_error("Inaccessible file:" + file);
		}
	}

	std::string ReadFile(const std::string& file)
	{
		auto fs = std::ifstream(file, std::ios::in | std::ios::binary | std::ios::ate);
		CheckFile(fs, file);
		auto data = std::string();
		auto size = fs.tellg();
		data.resize(size);
		fs.seekg(0, std::ios::beg);
		fs.read(&data[0], size);
		fs.close();
		return data;
	}

	void WriteFile(const std::string& file, const std::string& data)
	{
		auto fs = std::ofstream(file, std::ios::out | std::ios::binary);
		CheckFile(fs, file);
		fs.write(&data[0], data.size());
	}
}


void FConfig::LoadFromFile(const std::string& file)
{
	Archiver ar; 
	ar.Construct(FConfigUtil::ReadFile(file));
	this->Unmarshal(ar);
}

void FConfig::SafeToFile(const std::string& file)
{
	Archiver ar;
	this->Marshal(ar);
	FConfigUtil::WriteFile(file, ar.Archive());
}
