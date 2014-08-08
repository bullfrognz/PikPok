
//
//  File Name   :   JsonParser.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __JsonParser_H__
#define __JsonParser_H__

// Dependent Includes

// Local Includes
#include "Library/Dictionary.h"
#include "Library/List.h"
#include "Defines/Types.h"

// Library Includes
#include <string>

// Prototypes
struct JSON_parser_struct;

class CJsonParser
{

	// Member Types
public:

	enum ESettings
	{
		STACK_SIZE_MAX = 127
	};

	enum EValueType
	{
		INVALID_VALUE_TYPE,

		VALUE_TYPE_FIXED,
		VALUE_TYPE_REAL,
		VALUE_TYPE_STRING,
		VALUE_TYPE_BOOLEAN,
		VALUE_TYPE_ARRAY,
		VALUE_TYPE_OBJECT,
		VALUE_TYPE_NULL,

		MAX_VALUE_TYPE
	};

	struct TValue;

	struct TObject : public CDictionary<std::string, TValue>	{ ~TObject(); };
	struct TArray  : public CList<TValue>						{ ~TArray();  };

	struct TValue
	{
		union UValue
		{
			int64			i64Fixed;
			double			dReal;
			bool			bBoolean;
			std::string*	pString;
			TObject*		pObject;
			TArray*			pArray;
		};

		 TValue();
		 TValue(UValue _Value, EValueType _eValueType);
		~TValue();

		int				AsFixed()					{ return ((int)nValue.i64Fixed);	} 
		double			AsReal()					{ return (nValue.dReal);			} 
		bool			AsBool()					{ return (nValue.bBoolean);			} 
		std::string&	AsString()					{ return (*nValue.pString);			} 
		TObject&		AsObject()					{ return (*nValue.pObject);			} 
		TArray&			AsArray()					{ return (*nValue.pArray);			} 
		bool			IsType(EValueType _eType)	{ return (eType == _eType);			} 

		EValueType	eType;
		UValue		nValue;
	};

	// Member Functions
public:

	 CJsonParser();
	~CJsonParser();

	bool	ParseFile(c_char* _kcpFilePath);
	bool	ParseString(c_char* _kcpString);

	void	Print();

	TValue	BaseValue();

protected:

	void PrintObject(TObject& _rObject, int _iCurrentLevel);
	void PrintArray(TArray& _rArray, int _iCurrentLevel);

private:

	void SetObjectKey(c_char* _kcpKey);
	void InsertValue(EValueType _eValueType, void* _pValue);

	TValue*	GetCurrentStackValue();

	static void DeleteValue(TValue& _rValue);

	static int JsonParserCallback(void* _pContext, int _iValueType, const struct JSON_value_struct* _ktpValue);

	CJsonParser(const CJsonParser& _krJsonParser);
	CJsonParser& operator = (const CJsonParser& _krJsonParser);

	// Member Variables
protected:

private:

	JSON_parser_struct* m_tJsonParser;

	TValue				m_tStack[STACK_SIZE_MAX];

	int					m_iStackLevel;

	std::string			m_ObjectKey;

	CList<TValue*>		m_aLoadedValues;

};

#endif //__JsonParser_H__