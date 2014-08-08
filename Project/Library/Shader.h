
//
//  File Name   :   Shader.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __Shader_H__
#define __Shader_H__

// Dependent Includes

// Local Includes
#include "Library/List.h"
#include "Defines/Types.h"

// Library Includes

// Prototypes
class ITexture;
struct TMaterial;
struct TMatrix;
struct TVector3;
struct TVector4;
struct TColour;

class IShader
{

#define SHADERS_DIR "Shaders/"

	// Member Types
public:

	enum ESetting
	{
		LIGHTS_MAX		= 8,
		TEXTURES_MAX	= 8,
		MATERIALS_MAX	= 2,
	};

	enum EInput
	{
		INVALID_INPUT = -1,

		INPUT_POSITION3,
		INPUT_POSITION2,
		INPUT_NORMAL3,
		INPUT_NORMAL2,
		INPUT_TEXT_CORDS,

		MAX_INPUT
	};

	enum EInclude
	{
		INVALID_INCLUDE,

		INCLUDE_TRANSFORM	= 1 << 1,
		INCLUDE_LIGHTING	= 1 << 2,
		INCLUDE_TEXTURING	= 1 << 3,

		MAX_INCLUDE
	};

	// Member Functions
public:

	IShader() {};
	virtual ~IShader() {};

	virtual bool LoadFile(c_char* _kcpFilePath, uint _uiIncludes) = 0;

	virtual bool RegisterTechnique(c_char* _kcpName, EInput* _epInputLayout, uint _uiInputCount) = 0;
	virtual bool RegisterVariable(c_char* _kcpName)												 = 0;

	virtual bool SetVariable(c_char* _kcpName, const TMatrix& _kxrVariable)		= 0;
	virtual bool SetVariable(c_char* _kcpName, const TVector3& _kvrVariable)	= 0;
	virtual bool SetVariable(c_char* _kcpName, const TVector4& _kvrVariable)	= 0;
	virtual bool SetVariable(c_char* _kcpName, const TColour& _kvrVariable)		= 0;
	virtual bool SetVariable(c_char* _kcpName, float _fVariable)				= 0;
	virtual bool SetVariable(c_char* _kcpName, uint _uiVariable)				= 0;
	virtual bool SetVariable(c_char* _kcpName, int _iVariable)					= 0;
	virtual bool SetVariable(c_char* _kcpName, bool _bVariable)					= 0;
																				
	virtual bool GetVariable(c_char* _kcpName, TMatrix& _rVariable)	= 0;
	virtual bool GetVariable(c_char* _kcpName, float& _frVariable)	= 0;
	virtual bool GetVariable(c_char* _kcpName, int& _irVariable)	= 0;

	virtual uint Includes() const = 0;
																   
protected:														   

private:

	// Member Variables
protected:

private:

};

#endif //__Shader_H__

























//
//  Bryce Booth
//  Auckland
//  New Zealand
//
//  (c) 2011 Bryce Booth
//
//  File Name   :   Shader.h
//  Description :   --------------------------
//
//  Author      :   Bryce Booth
//  Mail        :   bryce.booth@mediadesign.school.nz
//


#pragma once


#ifndef __Shader_H__
#define __Shader_H__


// Local Includes
#include "Defines/Types.h"


// Library Includes
#include <string>
#include <vector>


// Types


// Constants
#define SHADERS_DIR "Shader Files\\"


// Prototypes
class IRenderer;
class CMatrix;
class TVector3;
class CVector4;
class CLight;
class ITexture;
struct TMaterial;
class CWorldMatrix;
class CViewMatrix;
struct TProjectionMatrix;


class IShader //: public CResource<IShader>
{

	// Member Types
public:





	// Member Functions
public:


			 IShader() {};
	virtual ~IShader() {};





protected:


private:


	IShader(const IShader& _krIShader);
	IShader& operator = (const IShader& _krIShader);


	// Member Variables
protected:


private:


};


#endif //__Shader_H__