#include "Archiver.hpp"

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"

#include <iostream>

namespace pt = boost::property_tree;


#define CAUGORY_PREFIX '@'


bool IsCategory(const std::string& str)
{
	if (str.length())
	{
		return str[0] == CAUGORY_PREFIX;
	}
	throw std::runtime_error("enpty name is unavailable");
}

bool IsPlane(const std::string& str)
{
	if (str.length())
	{
		return *str. begin() != '{'
		  || *++str.rbegin() != '}';
	}
	throw std::runtime_error("enpty name is unavailable");
}

std::string ShieldCategory(const std::string name)
{
	return CAUGORY_PREFIX + name;
}

std::string UnshieldCategory(const std::string name)
{
	return name.substr(1);
}



/** fillin the archive
 */
void ConstructTree(Archiver& ar, pt::ptree& tree, std::string prefix)
{
	for (auto& val : tree)
	{
		auto& name    = val.first;
		auto& subtree = val.second;

		if (IsCategory(name))
		{
			auto freeName = UnshieldCategory(name);
			auto next = prefix + '.' + freeName;
			ConstructTree(ar, subtree, next);
		}
		else if (!subtree.empty())
		{
			std::string s;
			std::stringstream ss(s);
			pt::write_json(ss, subtree);
			ar[prefix][name] = ss.str();
		}
		else
		{
			ar[prefix][name] = subtree.get_value<std::string>();
		}
	}
}

/** Contruct a ptree on base of the node
 */
pt::ptree ArchiveTree(Archiver::Node& node)
{
	pt::ptree tree;

	for (auto& pair : node.category.params)
	{
		auto& member = pair.first;
		auto& value = pair.second;
		if (IsPlane(value))
		{
			tree.add(member, value);
		}
		else
		{
			std::stringstream ss(value);
			
			pt::ptree subtree;
			pt::read_json(ss, subtree);
			tree.add_child(member, subtree);
		}
	}

	for (auto& child : node.children)
	{
		auto name    = ShieldCategory(child->name);
		auto subtree = ArchiveTree(*child.get());
		tree.add_child(name, subtree);
	}

	return tree;
}


/******************************************************************************
*								Archiver
******************************************************************************/


Archiver::Archiver()
	: root(*this, "")
{}

void Archiver::Construct(const std::string& json)
{
	if (json == "") return;

	std::stringstream ss(json);
	
	pt::ptree tree;
	pt::read_json(ss, tree);
	ConstructTree(*this, tree, "");

	// to avoid double assign exceptions
	members.clear(); 
}

std::string Archiver::Archive()
{
	auto tree = ArchiveTree(root);

	std::string s;
	std::stringstream ss(s);
	pt::write_json(ss, tree);
	return ss.str();
}

Archiver::Category& Archiver::operator[](std::string category)
{
	return GetCategory(category);
}

bool Archiver::RegisterMember(const std::string& member)
{
	if (!members.count(member))
	{
		members.emplace(member);
		return true;
	}
	else return false;
}

Archiver::Category& Archiver::GetCategory(const std::string& name)
{
	auto tokens = GetTokens(name);

	Node* node = &root;
	for (auto& token : tokens)
	{
		if (token != "")
		{
			node = &node->GetOrCreate(token);
		}
	}
	return node->category;
}

std::vector<std::string> Archiver::GetTokens(const std::string& name)
{
	std::vector<std::string> tokens;
	std::stringstream ss(name);
	std::string s;
	while (std::getline(ss, s, '.'))
	{
		tokens.emplace_back(s);
	}
	return tokens;
}

/******************************************************************************
*								Category
******************************************************************************/

Archiver::Category::Category(Archiver& owner)
	: owner(owner)
{}

std::string& Archiver::Category::operator[](std::string member)
{
	if (!owner.RegisterMember(member))
	{
		throw std::runtime_error("The parametr already contains");
	}
	return params[member];
}

/******************************************************************************
*								Node
******************************************************************************/

Archiver::Node::Node(Archiver& owner, const std::string& name)
	: category(owner)
	, owner   (owner)
	, name    (name)
{}

Archiver::Node& Archiver::Node::GetOrCreate(const std::string& subCategory)
{
	auto itr = children.begin();
	auto end = children.end();
	auto pos = std::find_if(itr, end, [&subCategory](auto& ptr)
	{
		return ptr->name == subCategory;
	});

	if (pos == end)
	{
		auto ptr = std::make_unique<Node>(owner, subCategory);
		auto raw = ptr.get();
		children.emplace_back(std::move(ptr));
		return *raw;
	}
	else return *pos->get();
}
