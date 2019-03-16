#ifndef TYPECONVERSION_HPP
#define TYPECONVERSION_HPP
#pragma once

#include <sstream>

/******************************************************************************
*									Type2String
******************************************************************************/

template<typename _T, bool IsArchibale, bool IsPointer>
struct Type2Srting
{
	std::string operator()(_T& t)
	{
		static_assert(false);
	}
};

template<typename _T>
struct Type2Srting<_T, false, false>
{
	std::string operator()(_T& t)
	{
		return (std::ostringstream() << t).str();
	}
};

template<>
struct Type2Srting<std::string, false, false>
{
	std::string operator()(std::string& t)
	{
		return t;
	}
};

template<typename _T>
struct Type2Srting<_T, true, false>
{
	std::string operator()(_T& t)
	{
		Archiver ar;
		t.__Archive(ar, true);
		return ar.Archive();
	}
};

template<typename _T>
struct Type2Srting<_T, true, true>
{
	std::string operator()(_T t)
	{
		if (t)
		{
			Archiver ar;
			t->__Archive(ar, true);
			return ar.Archive();
		}
		return std::string();
	}
};

/******************************************************************************
*									String2Type
******************************************************************************/

template<typename _T, bool IsArchibale, bool IsPointer>
struct String2Type
{
	void operator()(_T& t, const std::string& str)
	{
		static_assert(false);
	}
};

template<typename _T>
struct String2Type<_T, false, false>
{
	void operator()(_T& t, const std::string& str)
	{
		std::stringstream(str) >> t;
	}
};

template<typename _T>
struct String2Type<_T, true, false>
{
	void operator()(_T& t, const std::string& str)
	{
		Archiver ar;
		ar.Construct(str);
		t.__Archive(ar, false);
	}
};

template<typename _T>
struct String2Type<_T, true, true>
{
	void operator()(_T t, const std::string& str)
	{
		if (t)
		{
			Archiver ar;
			ar.Construct(str);
			t->__Archive(ar, false);
		}
	}
};


#endif //!TYPECONVERSION_HPP
