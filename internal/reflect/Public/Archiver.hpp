#ifndef ARCHIVER_HPP
#define ARCHIVER_HPP
#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

#include <memory>
#include "common.hpp"


class Archiver
{
public:
	struct Category
	{
		using ssMap = std::unordered_map<std::string, std::string>;
	public:
		ssMap    params;
		Archiver& owner;

	public:
		Category(Archiver& owner);

		std::string& operator[](std::string member);
	};

public:
	struct Node
	{
		using Nodes = std::vector<UNIQUE(Node)>;
	public:
		std::string  name; // subcategory name
		Category category; // category members
		Nodes    children; // child subcategories
		Archiver&   owner;

	public:
		Node(Archiver& owner, const std::string& name);

		Node& GetOrCreate(const std::string& subCategory);
	};

public: //~~~~~~~~~~~~~~~~~~~| interface
	Archiver();

	void Construct(const std::string& json);
	std::string Archive();

	Category& operator[](std::string category);

	bool RegisterMember(const std::string& member);

private:
	Node root;
	std::unordered_set<std::string> members;

private:
	Category& GetCategory(const std::string& name);

	std::vector<std::string> GetTokens(const std::string& name);
};


#endif //!ARCHIVER_HPP
