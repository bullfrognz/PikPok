
//
//  File Name   :   JsonParser.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "JsonParser.h"

// Local Includes
#include "Framework/Debug.h"
#include "Framework/FileSystem.h"
#include "Framework/File.h"

// Library Includes
#include "Plugins/Json_Parser/JSON_parser.h"

// Static Initialisers

// Prototypes
void* JsonMalloc(size_t _uiSize);

void JsonFree(void* _pMemory);

// Implementation

CJsonParser::TValue::TValue()
: eType(INVALID_VALUE_TYPE)
{
	FW_MEMZERO(&nValue, sizeof(TValue::UValue));
}

CJsonParser::TValue::TValue(UValue _Value, EValueType _eValueType)
: nValue(_Value)
, eType(_eValueType)
{
	// Empty
}

CJsonParser::TValue::~TValue()
{
	/*
	if (IsType(VALUE_TYPE_OBJECT))
	{
		FW_DELETE(nValue.pObject);
	}
	else if (IsType(VALUE_TYPE_ARRAY))
	{
		FW_DELETE(nValue.pArray);
	}
	else if (IsType(VALUE_TYPE_STRING))
	{
		FW_DELETE(nValue.pString);
	}
	*/
}

CJsonParser::TObject::~TObject()
{
	TObject::TEntries tEntries = GetEntries();

	while (!tEntries.AtEnd())
	{
		DeleteValue(tEntries.Value());

		++ tEntries;
	}
}

CJsonParser::TArray::~TArray()
{
	TArray::TItems tItems = Items();

	while (!tItems.End())
	{
		DeleteValue(tItems.Value());

		++ tItems;
	}
}

CJsonParser::CJsonParser()
: m_tJsonParser(0)
, m_iStackLevel(-1)
{
	JSON_config tJsonConfig;
	tJsonConfig.callback = &CJsonParser::JsonParserCallback;
	tJsonConfig.callback_ctx = this;
	tJsonConfig.depth = 127;
	tJsonConfig.allow_comments = 1;
	tJsonConfig.handle_floats_manually = 0;

	init_JSON_config(&tJsonConfig);
}

CJsonParser::~CJsonParser()
{
	DeleteValue(m_tStack[0]);
}

bool CJsonParser::ParseFile(c_char* _kcpFilePath)
{
	bool bReturn = false;

	// Open json file
	IFile* pJsonFile = FILESYSTEM.InstanceFile();
	
	bool bOpened = pJsonFile->Open(_kcpFilePath, IFile::OPEN_ATTRIBRUTE_READ); //IFile::OPEN_ATTRIBRUTE_BINARY | 

	if (!bOpened)
	{
		DEBUG_ERROR("Could not open json file to parse. File may not exist. FilePath(%s)", _kcpFilePath);
	}
	else
	{
		// Create read buffer
		uint uiFileSize = pJsonFile->Size();

		char* cpFileContents = new char[uiFileSize];
		FW_MEMZERO(cpFileContents, uiFileSize);

		// Read json file
		bool bFileRead = pJsonFile->Read(cpFileContents, uiFileSize);

		if (!bFileRead)
		{
			DEBUG_ERROR("Failed to read json file. FilePath(%s)", _kcpFilePath);
		}
		else
		{
			bReturn = ParseString(cpFileContents);
		}

		FW_ADELETE(cpFileContents);
	}

	FW_DELETE(pJsonFile);

	return (bReturn);
}

bool CJsonParser::ParseString(c_char* _kcpString)
{
	DeleteValue(m_tStack[0]);

	JSON_config tJsonConfig;
	tJsonConfig.callback = &CJsonParser::JsonParserCallback;
	tJsonConfig.callback_ctx			= this;
	tJsonConfig.depth					= 127;
	tJsonConfig.malloc					= &JsonMalloc;
	tJsonConfig.free					= &JsonFree;
	tJsonConfig.allow_comments			= 1;
	tJsonConfig.handle_floats_manually	= 0;

	uint uiLength = strlen(_kcpString);
	m_tJsonParser = new_JSON_parser(&tJsonConfig);

	for (uint i = 0; i < uiLength ; ++ i) 
	{
		if (!JSON_parser_char(m_tJsonParser, _kcpString[i]))
		{
			delete_JSON_parser(m_tJsonParser);
			DEBUG_ERROR("JSON_parser_char: syntax error, byte %d", i);

			return (false);
		}
	}

	if (!JSON_parser_done(m_tJsonParser)) 
	{
		delete_JSON_parser(m_tJsonParser);
		DEBUG_ERROR("JSON_parser_end: syntax error");

		return (false);
	}

	delete_JSON_parser(m_tJsonParser);

	return (true);
}

void  CJsonParser::Print()
{
	int uiStackLevel = 0;

	PrintObject(*m_tStack[0].nValue.pObject, 0);
}

CJsonParser::TValue CJsonParser::BaseValue()
{
	return (m_tStack[0]);
}

void CJsonParser::PrintObject(TObject& _rObject, int _iCurrentLevel)
{
	CDictionary<std::string, TValue>::TEntries tEntries = _rObject.GetEntries();

	DEBUG_WRITE("{");

	std::string Key;

	while (!tEntries.AtEnd())
	{
		Key = "";

		for (int i = 0; i < _iCurrentLevel; ++ i)
			Key += "\t";

		Key += tEntries.Key();

		switch (tEntries.Value().eType)
		{
		case VALUE_TYPE_FIXED:
			Key += " : %d,";
			DEBUG_WRITE(Key.c_str(), tEntries.Value().AsFixed());
			break;

		case VALUE_TYPE_REAL:
			Key += " : %.3f,";
			DEBUG_WRITE(Key.c_str(), tEntries.Value().AsReal());
			break;

		case VALUE_TYPE_STRING:
			Key += " : %s,";
			DEBUG_WRITE(Key.c_str(), tEntries.Value().AsString().c_str());
			break;

		case VALUE_TYPE_BOOLEAN:
			Key += " : %d,";
			DEBUG_WRITE(Key.c_str(), tEntries.Value().AsBool());
			break;

		case VALUE_TYPE_ARRAY:
			Key += " : ";
			DEBUG_WRITE(Key.c_str());
			PrintArray(*tEntries.Value().nValue.pArray, _iCurrentLevel + 1);
			DEBUG_WRITE(",");
			break;

		case VALUE_TYPE_OBJECT:
			Key += " : ";
			DEBUG_WRITE(Key.c_str());
			PrintObject(*tEntries.Value().nValue.pObject, _iCurrentLevel + 1);
			DEBUG_WRITE(",");
			break;

		case VALUE_TYPE_NULL:
			Key += " : null,";
			DEBUG_WRITE(Key.c_str());
			break;
		}

		++ tEntries;
	}

	DEBUG_WRITE("}");
}

void CJsonParser::PrintArray(TArray& _rArray, int _iCurrentLevel)
{
	CList<TValue>::TItems tItems = _rArray.Items();

	DEBUG_WRITE("[");

	std::string Tabbing;

	while (!tItems.End())
	{
		Tabbing = "";

		for (int i = 0; i < _iCurrentLevel; ++ i)
			Tabbing += "\t";

		switch (tItems.Value().eType)
		{
		case VALUE_TYPE_FIXED:
			Tabbing += "%d,";
			DEBUG_WRITE(Tabbing.c_str(), tItems.Value().AsFixed());
			break;

		case VALUE_TYPE_REAL:
			Tabbing += "%.3f,";
			DEBUG_WRITE(Tabbing.c_str(), tItems.Value().AsReal());
			break;

		case VALUE_TYPE_STRING:
			Tabbing += "%s,";
			DEBUG_WRITE(Tabbing.c_str(), tItems.Value().AsString().c_str());
			break;

		case VALUE_TYPE_BOOLEAN:
			Tabbing += "%d,";
			DEBUG_WRITE(Tabbing.c_str(), tItems.Value().AsBool());
			break;

		case VALUE_TYPE_ARRAY:
			PrintArray(*tItems.Value().nValue.pArray, _iCurrentLevel + 1);
			DEBUG_WRITE(",");
			break;

		case VALUE_TYPE_OBJECT:
			Tabbing += ",";
			DEBUG_WRITE(Tabbing.c_str());
			PrintObject(*tItems.Value().nValue.pObject, _iCurrentLevel + 1);
			DEBUG_WRITE(",");
			break;

		case VALUE_TYPE_NULL:
			Tabbing += "null,";
			DEBUG_WRITE(Tabbing.c_str());
			break;
		}

		++ tItems;
	}

	DEBUG_WRITE("]");
}

void CJsonParser::SetObjectKey(c_char* _kcpKey)
{
	m_ObjectKey = _kcpKey;
}

void CJsonParser::InsertValue(EValueType _eValueType, void* _pValue)
{
	TValue tNewValue;
	tNewValue.eType = _eValueType;
	
	switch (_eValueType)
	{
	case VALUE_TYPE_FIXED:
		tNewValue.nValue.i64Fixed = *reinterpret_cast<int64*>(_pValue);
		break;

	case VALUE_TYPE_REAL:
		tNewValue.nValue.dReal = *reinterpret_cast<double*>(_pValue);
		break;

	case VALUE_TYPE_STRING:
		tNewValue.nValue.pString = reinterpret_cast<std::string*>(_pValue);
		break;

	case VALUE_TYPE_BOOLEAN:
		tNewValue.nValue.bBoolean = *reinterpret_cast<bool*>(_pValue);
		break;

	case VALUE_TYPE_ARRAY:
		tNewValue.nValue.pArray = reinterpret_cast<TArray*>(_pValue);
		break;

	case VALUE_TYPE_OBJECT:
		tNewValue.nValue.pObject = reinterpret_cast<TObject*>(_pValue);
		break;
	}

	if (GetCurrentStackValue()->IsType(VALUE_TYPE_ARRAY))
	{
		GetCurrentStackValue()->AsArray().Add(tNewValue);
	}
	else if (GetCurrentStackValue()->IsType(VALUE_TYPE_OBJECT))
	{
		GetCurrentStackValue()->AsObject().Add(m_ObjectKey, tNewValue);
	}
	else
	{
		DEBUG_ERROR("Invalid current container type");
	}
}

CJsonParser::TValue* CJsonParser::GetCurrentStackValue()
{
	return (&m_tStack[m_iStackLevel]);
}

void CJsonParser::DeleteValue(TValue& _rValue)
{
	if (_rValue.eType == VALUE_TYPE_OBJECT)
	{
		FW_DELETE(_rValue.nValue.pObject);
	}
	else if (_rValue.eType == VALUE_TYPE_ARRAY)
	{
		FW_DELETE(_rValue.nValue.pArray);
	}
	else if (_rValue.eType == VALUE_TYPE_STRING)
	{
		FW_DELETE(_rValue.nValue.pString);
	}
}

int  CJsonParser::JsonParserCallback(void* _pContext, int _iValueType, const struct JSON_value_struct* _ktpValue)
{
	CJsonParser& rInstance = *reinterpret_cast<CJsonParser*>(_pContext);

	switch (_iValueType)
	{
	case JSON_T_ARRAY_BEGIN:
		{
			TArray* pNewArray = new CJsonParser::TArray();

			if (rInstance.m_iStackLevel >= 0)
				rInstance.InsertValue(VALUE_TYPE_ARRAY, pNewArray);

			++ rInstance.m_iStackLevel;
			rInstance.GetCurrentStackValue()->eType = CJsonParser::VALUE_TYPE_ARRAY;
			rInstance.GetCurrentStackValue()->nValue.pArray = pNewArray;
		}
		break;

	case JSON_T_ARRAY_END:
		-- rInstance.m_iStackLevel;
		break;

	case JSON_T_OBJECT_BEGIN:
		{
			TObject* pNewObject = new CJsonParser::TObject();

			if (rInstance.m_iStackLevel >= 0)
				rInstance.InsertValue(VALUE_TYPE_OBJECT, pNewObject);

			++ rInstance.m_iStackLevel;
			rInstance.GetCurrentStackValue()->eType = CJsonParser::VALUE_TYPE_OBJECT;
			rInstance.GetCurrentStackValue()->nValue.pObject = pNewObject;
		}
		break;

	case JSON_T_OBJECT_END:
		-- rInstance.m_iStackLevel;
		break;

	case JSON_T_INTEGER:
		{
			int64 i64NewInt = _ktpValue->vu.integer_value;
			rInstance.InsertValue(VALUE_TYPE_FIXED, &i64NewInt);
		}
		break;
		 
	case JSON_T_FLOAT:
		{
			double dNewDouble = _ktpValue->vu.float_value;
			rInstance.InsertValue(VALUE_TYPE_REAL, &dNewDouble);
		}
		break;

	case JSON_T_NULL:
		{
			rInstance.InsertValue(VALUE_TYPE_NULL, 0);
		}
		break;

	case JSON_T_TRUE:
		{
			bool bNewBool = true;
			rInstance.InsertValue(VALUE_TYPE_BOOLEAN, &bNewBool);
		}
		break;

	case JSON_T_FALSE:
		{
			bool bNewBool = false;
			rInstance.InsertValue(VALUE_TYPE_BOOLEAN, &bNewBool);
		}
		break;

	case JSON_T_KEY:
		rInstance.SetObjectKey(_ktpValue->vu.str.value);
		break;   

	case JSON_T_STRING:
		{
			std::string* sString = new std::string;
			*sString = _ktpValue->vu.str.value;
			//char* cpNewString = new char[_ktpValue->vu.str.length + 1];
			//FW_MEMCOPY(cpNewString, _ktpValue->vu.str.value, sizeof(char) * _ktpValue->vu.str.length + 1);
			rInstance.InsertValue(VALUE_TYPE_STRING, sString);
		}
		break;

	default:
		DEBUG_ERROR("Unknown json value type. Type(%d)", _iValueType);
		break;
	}

	return (1);
}

void* JsonMalloc(size_t _uiSize)
{
	return (malloc(_uiSize));
}

void JsonFree(void* _pMemory)
{
	free(_pMemory);
}