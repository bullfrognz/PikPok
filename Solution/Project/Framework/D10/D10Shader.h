
//
//  File Name   :   D10Shader.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __D10Shader_H__
#define __D10Shader_H__

// Dependent Includes
#include "Library/Shader.h"


// Local Includes
#include "Defines/Types.h"
#include "Library/WorldMatrix.h"
#include "Library/ViewMatrix.h"
#include "Library/ProjectionMatrix.h"

// Library Includes
#include <map>
#include <string>
#include <d3d10.h>

// Prototypes
struct TLight;
class CD10Renderer;

class CD10Shader : public IShader
{

	// Member Types
public:

	enum ETransform
	{
		TRANSFORM_WORLD = 1 << 0,
		TRANSFORM_VIEW = 1 << 1,
		TRANSFORM_PROJECTION = 1 << 2,
	};

	struct TTechniqueInfo
	{
		ID3D10EffectTechnique*	pD10Technique;
		ID3D10InputLayout*		pD10InputLayout;
	};

	// Member Functions
public:

			 CD10Shader(CD10Renderer& _rD10Renderer);
	virtual ~CD10Shader();

	virtual bool LoadFile(c_char* _kcpFilePath, uint _uiIncludes);

	virtual bool RegisterTechnique(c_char* _kcpName, EInput* _epInputLayout, uint _uiInputCount);
	virtual bool RegisterVariable(c_char* _kcpName);

	virtual bool SetTextures(ITexture** _ppTextures, uint _uiNumTextures);

	virtual bool SetVariable(c_char* _kcpName, const TMatrix& _kxrVariable);
	virtual bool SetVariable(c_char* _kcpName, const TVector3& _kvrVariable);
	virtual bool SetVariable(c_char* _kcpName, const TVector4& _kvrVariable);
	virtual bool SetVariable(c_char* _kcpName, const TColour& _kvrVariable);
	virtual bool SetVariable(c_char* _kcpName, float _fVariable);
	virtual bool SetVariable(c_char* _kcpName, uint _uiVariable);
	virtual bool SetVariable(c_char* _kcpName, int _iVariable);
	virtual bool SetVariable(c_char* _kcpName, bool _bVariable);

	virtual bool GetVariable(c_char* _kcpName, TMatrix& _rVariable);
	virtual bool GetVariable(c_char* _kcpName, float& _frVariable);
	virtual bool GetVariable(c_char* _kcpName, int& _irVariable);

	virtual uint Includes() const;

	bool SetLights(const TLight** _kppLights, uint _uiNumLights, const TVector3& _kvrCameraPosition);
	bool SetTransformations(uint _uiTransforms, const TWorldMatrix& _kxrWorld, const TViewMatrix& _kxrView, const TProjectionMatrix& _kxrProjection);

	TTechniqueInfo&	GetTechniqueInfo();
	bool			GetD10EffectVariable(c_char* _kcpName, ID3D10EffectVariable*& _prD10EffectVariable);

protected:

	bool CreateInputLayout(ID3D10EffectTechnique* _pTechnique, D3D10_INPUT_ELEMENT_DESC* _tpVetexDesc, uint _uiNumElements, ID3D10InputLayout*& _prInputLayout);

	bool CompileInputLayout(EInput* _epInputLayout, uint _uiInputCount, std::vector<D3D10_INPUT_ELEMENT_DESC>& _rInputElementDescs);

private:

	bool LoadIncludes();

	CD10Shader(const CD10Shader& _krD10Shader);
	CD10Shader& operator = (const CD10Shader& _krD10Shader);

	// Member Variables
protected:

private:

	TTechniqueInfo	m_tTechniqueInfo;

	CD10Renderer&	m_rD10Renderer;

	char*			m_cpFile;
	ID3D10Effect*	m_pD10Shader;

	uint			m_uiIncludes;

	std::map<std::string, ID3D10EffectVariable*> m_Variables;

};

#endif //__D10Shader_H__