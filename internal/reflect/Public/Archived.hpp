#ifndef ARCHIVED_HPP
#define ARCHIVED_HPP
#pragma once

#include <string>
#include <sstream>

#include "Archiver.hpp"
#include "TypeConversion.hpp"


/** A base type for all classes / structs in engine
 */
struct Archived
{
public: //~~~~~~~~~~~~~~| Generated body
	 Archived() = default;
	~Archived() = default;

public:
	/** @return a class name
	 *  NOTE: use a GENRATED_BODY() macro */
	virtual std::string __GetClassName() const = 0;

public:
	void Marshal  (Archiver& ar) { __Archive(ar, true); }
	void Unmarshal(Archiver& ar) { __Archive(ar, false); }
	/** Archive / Dearchive the class' mebers
	 * @param ar       - archve 
	 * @param bForward - if true - archive else - disarchive */
	virtual void __Archive(Archiver& ar, bool bForvward) {}
};


/******************************************************************************
*									Utiles
******************************************************************************/

template<class _B, class _T, bool bPointer> 
struct is_base_of : public std::is_base_of<_B, _T> {};

template<class _B, class _T>
struct is_base_of<_B, _T, true> : public std::is_base_of<_B, std::remove_pointer_t<_T>> {};

#define SMTH2SMTH_ARGS(VAR)  decltype(VAR)								\
	, is_base_of<														\
		  Archived														\
		, decltype(VAR) 												\
		, std::is_pointer<decltype(VAR)>::value							\
		>::value														\
	, std::is_pointer<decltype(VAR)>::value								\
/**/

/******************************************************************************
*									Defines
******************************************************************************/

#define GENERATED_BODY(CLASS, SUPER)									\
	public:																\
		virtual std::string __GetClassName() const override				\
		{																\
			return std::string(#CLASS);									\
		}																\
		using Super = SUPER;											\
/**/

#define ARCHIVATION_BEGIN()												\
	public:																\
	virtual void __Archive(Archiver& ar, bool bForvward) override		\
	{																	\
		Super::__Archive(ar, bForvward);								\
/**/

#define ARCHIVATION_MEMBER(CATEGORY, VAR)								\
	if (bForvward)														\
	{																	\
		ar[CATEGORY][#VAR] = Type2Srting<SMTH2SMTH_ARGS(VAR)>()(VAR);	\
	}																	\
	else																\
	{																	\
		String2Type<SMTH2SMTH_ARGS(VAR)>()(VAR, ar[CATEGORY][#VAR]);	\
	}																	\
/**/

#define ARCHIVATION_END()												\
	}																	\
/**/

#define GENERATED_ARCHIVATION_BODY(CLASS, SUPER)						\
	GENERATED_BODY(CLASS, SUPER)										\
	ARCHIVATION_BEGIN()													\
/**/

#endif //!ARCHIVED_HPP
