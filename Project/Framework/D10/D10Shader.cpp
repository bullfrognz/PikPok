
//
//  File Name   :   CD10Shader.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "D10Shader.h"

// Local Includes
#include "Framework/D10/D10Renderer.h"
#include "Framework/D10/D10Texture.h"
#include "Framework/Debug.h"
#include "Library/StrUtilities.h"
#include "Library/Vector4.h"
#include "Library/Vector3.h"
#include "Library/Light.h"

// Library Includes

// Static Initialisers

// Implementation

CD10Shader::CD10Shader(CD10Renderer& _rD10Renderer)
: m_rD10Renderer(_rD10Renderer)
, m_cpFile(null)
, m_pD10Shader(null)
, m_uiIncludes(0)
{
	// Empty
}

CD10Shader::~CD10Shader()
{
	FW_RELEASE(m_tTechniqueInfo.pD10InputLayout);

	FW_ADELETE(m_cpFile);
	FW_RELEASE(m_pD10Shader);
}

bool CD10Shader::LoadFile(c_char* _kcpFilePath, uint _uiIncludes)
{
	StrUtilities::Copy(_kcpFilePath, m_cpFile);
	m_uiIncludes = _uiIncludes;

	// Set shader compile flags
	ulong ulShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;

#if defined(DEBUG) || defined(_DEBUG)

	ulShaderFlags |= D3D10_SHADER_DEBUG;
	ulShaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
	//ulShaderFlags |= D3D10_SHADER_WARNINGS_ARE_ERRORS;

#endif

	ID3D10Blob* pCompilationErrors = null;
	HRESULT hResult = D3DX10CreateEffectFromFile(_kcpFilePath, 0, 0, "fx_4_0", ulShaderFlags, 0, m_rD10Renderer.GetDevice(),
												 0, 0, &m_pD10Shader, &pCompilationErrors, 0);

	if (FAILED(hResult))
	{
		if (pCompilationErrors != null)
		{
			DEBUG_ERROR("Shader failed to compule. ShaderFile(%s) ErrorMessage(%s)", _kcpFilePath, static_cast<char*>(pCompilationErrors->GetBufferPointer()));

			FW_RELEASE(pCompilationErrors);
		}

		return (false);
	}

	VALIDATE(LoadIncludes());

	return (true);
}

bool CD10Shader::RegisterTechnique(c_char* _kcpName, EInput* _epInputLayout, uint _uiInputCount)
{
	std::vector<D3D10_INPUT_ELEMENT_DESC> InputElementDescs;

	VALIDATE(CompileInputLayout(_epInputLayout, _uiInputCount, InputElementDescs));

	ID3D10EffectTechnique* pD10Technique = m_pD10Shader->GetTechniqueByName(_kcpName);

	ID3D10InputLayout* pInputLayout = 0;
	VALIDATE(CreateInputLayout(pD10Technique, &(*InputElementDescs.begin()), InputElementDescs.size(), pInputLayout));

	m_tTechniqueInfo.pD10Technique		= pD10Technique;
	m_tTechniqueInfo.pD10InputLayout	= pInputLayout;

	return (true);
}

bool CD10Shader::RegisterVariable(c_char* _kcpName)
{
	ID3D10EffectVariable* pVariable = m_pD10Shader->GetVariableByName(_kcpName);

	m_Variables.insert(std::pair<std::string, ID3D10EffectVariable*>(_kcpName, pVariable));

	return (true);
}

bool CD10Shader::SetTextures(ITexture** _ppTextures, uint _uiNumTextures)
{
	std::map<std::string, ID3D10EffectVariable*>::iterator ResourcesContainer = m_Variables.find("g_TextureResources");
	uint uiNumValidTextures = 0;

	if (ResourcesContainer == m_Variables.end())
	{
		DEBUG_ERROR("Shader file has not included texture helper effect file");
		return (false);
	}

	for (uint i = 0; i < IShader::TEXTURES_MAX && uiNumValidTextures < _uiNumTextures; ++i)
	{
		if (_ppTextures[i] == null)
			continue;

		ID3D10ShaderResourceView* pShaderResourceView = static_cast<CD10Texture*>(_ppTextures[i])->GetResourceView();

		(*ResourcesContainer).second->GetElement(uiNumValidTextures)->AsShaderResource()->SetResource(pShaderResourceView);

		++uiNumValidTextures;
	}

	VALIDATE(SetVariable("g_iTextureCount", uiNumValidTextures));

	return (true);
}

bool CD10Shader::SetVariable(c_char* _kcpName, const TMatrix& _kxrVariable)
{
	ID3D10EffectVariable* pD10EffectVariable = null;
	VALIDATE(GetD10EffectVariable(_kcpName, pD10EffectVariable));

	std::map<std::string, ID3D10EffectVariable*>::iterator VariableContainer = m_Variables.find(_kcpName);
	
	c_float* kfpMatrix = reinterpret_cast<const float*>(&_kxrVariable);
	pD10EffectVariable->AsMatrix()->SetMatrix(const_cast<float*>(kfpMatrix));

	return (true);
}

bool CD10Shader::SetVariable(c_char* _kcpName, const TVector3& _kvrVariable)
{
	return (SetVariable(_kcpName, TVector4(_kvrVariable.x,
										   _kvrVariable.y,
										   _kvrVariable.z,
									       0.0f)));
}

bool CD10Shader::SetVariable(c_char* _kcpName, const TVector4& _kvrVariable)
{
	ID3D10EffectVariable* pD10EffectVariable = null;
	VALIDATE(GetD10EffectVariable(_kcpName, pD10EffectVariable));

	c_float* kfpMatrix = reinterpret_cast<const float*>(&_kvrVariable);
	pD10EffectVariable->AsVector()->SetFloatVector(const_cast<float*>(kfpMatrix));

	return (true);
}

bool CD10Shader::SetVariable(c_char* _kcpName, const TColour& _kvrVariable)
{
	ID3D10EffectVariable* pD10EffectVariable = null;
	VALIDATE(GetD10EffectVariable(_kcpName, pD10EffectVariable));

	c_float* kfpMatrix = reinterpret_cast<const float*>(&_kvrVariable);
	pD10EffectVariable->AsVector()->SetFloatVector(const_cast<float*>(kfpMatrix));

	return (true);
}

bool CD10Shader::SetVariable(c_char* _kcpName, float _fVariable)
{
	ID3D10EffectVariable* pD10EffectVariable = null;
	VALIDATE(GetD10EffectVariable(_kcpName, pD10EffectVariable));

	pD10EffectVariable->AsScalar()->SetFloat(_fVariable);

	return (true);
}

bool CD10Shader::SetVariable(c_char* _kcpName, uint _uiVariable)
{
	return (SetVariable(_kcpName, static_cast<int>(_uiVariable)));
}

bool CD10Shader::SetVariable(c_char* _kcpName, int _iVariable)
{
	ID3D10EffectVariable* pD10EffectVariable = null;
	VALIDATE(GetD10EffectVariable(_kcpName, pD10EffectVariable));

	pD10EffectVariable->AsScalar()->SetInt(_iVariable);

	return (true);
}

bool CD10Shader::SetVariable(c_char* _kcpName, bool _bVariable)
{
	ID3D10EffectVariable* pD10EffectVariable = null;
	VALIDATE(GetD10EffectVariable(_kcpName, pD10EffectVariable));
	
	pD10EffectVariable->AsScalar()->SetBool(_bVariable);

	return (true);
}

bool CD10Shader::GetVariable(c_char* _kcpName, TMatrix& _rVariable)
{
	ID3D10EffectVariable* pD10EffectVariable = null;
	VALIDATE(GetD10EffectVariable(_kcpName, pD10EffectVariable));

	pD10EffectVariable->AsMatrix()->GetMatrix(reinterpret_cast<float*>(&_rVariable));

	return (true);
}

bool CD10Shader::GetVariable(c_char* _kcpName, float& _frVariable)
{
	ID3D10EffectVariable* pD10EffectVariable = null;
	VALIDATE(GetD10EffectVariable(_kcpName, pD10EffectVariable));

	pD10EffectVariable->AsScalar()->GetFloat(&_frVariable);

	return (true);
}

bool CD10Shader::GetVariable(c_char* _kcpName, int& _irVariable)
{
	ID3D10EffectVariable* pD10EffectVariable = null;
	VALIDATE(GetD10EffectVariable(_kcpName, pD10EffectVariable));

	pD10EffectVariable->AsScalar()->GetInt(&_irVariable);

	return (true);
}

uint CD10Shader::Includes() const
{
	return (m_uiIncludes);
}

bool CD10Shader::SetLights(const TLight** _kppLights, uint _uiNumLights, const TVector3& _kvrCameraPosition)
{
	if ((m_uiIncludes & INCLUDE_LIGHTING) == 0)
		return (false);

	ID3D10EffectVariable* pD10EffectVariable = 0;
	VALIDATE(GetD10EffectVariable("g_tLights", pD10EffectVariable));

	uint uiNumValidLights = 0;

	for (uint i = 0; i < IShader::LIGHTS_MAX && uiNumValidLights < _uiNumLights; ++i)
	{
		if (_kppLights[i] == null)
			continue;

		pD10EffectVariable->GetElement(uiNumValidLights)->SetRawValue(const_cast<TLight**>(_kppLights)[i], 0, sizeof(TLight));

		++uiNumValidLights;
	}

	if (uiNumValidLights > 0)
	{
		VALIDATE(SetVariable("g_vCameraPosition", _kvrCameraPosition));
	}

	VALIDATE(SetVariable("g_iNumLights", uiNumValidLights));

	return (true);
}

bool CD10Shader::SetTransformations(uint _uiTransforms, const TWorldMatrix& _kxrWorld, const TViewMatrix& _kxrView, const TProjectionMatrix& _kxrProjection)
{
	if (_uiTransforms & TRANSFORM_WORLD)
		VALIDATE(SetVariable("g_xWorld", _kxrWorld));

	if (_uiTransforms & TRANSFORM_VIEW)
		VALIDATE(SetVariable("g_xView", _kxrView));

	if (_uiTransforms & TRANSFORM_PROJECTION)
		VALIDATE(SetVariable("g_xProjection", _kxrProjection));

	TMatrix xWorldViewProj(_kxrWorld);
	xWorldViewProj *= _kxrView;
	xWorldViewProj *= _kxrProjection;

	VALIDATE(SetVariable("g_xWorldViewProjection", xWorldViewProj));

	return (true);
}

CD10Shader::TTechniqueInfo&	CD10Shader::GetTechniqueInfo()
{
	return (m_tTechniqueInfo);
}

bool CD10Shader::GetD10EffectVariable(c_char* _kcpName, ID3D10EffectVariable*& _prD10EffectVariable)
{
	std::map<std::string, ID3D10EffectVariable*>::iterator VariableContainer = m_Variables.find(_kcpName);

	if (VariableContainer == m_Variables.end())
	{
		DEBUG_ERROR("Shader variable has not been registered. VariableName(%s)", _kcpName);
		return (false);
	}

	_prD10EffectVariable = (*VariableContainer).second;

	return (true);
}

bool CD10Shader::CreateInputLayout(ID3D10EffectTechnique* _pTechnique, D3D10_INPUT_ELEMENT_DESC* _tpVetexDesc, uint _uiNumElements, ID3D10InputLayout*& _prInputLayout)
{
	D3D10_PASS_DESC tPassDesc;
	FW_MEMZERO(&tPassDesc, sizeof(D3D10_PASS_DESC));

	HRESULT hResult = _pTechnique->GetPassByIndex(0)->GetDesc(&tPassDesc);

	if (FAILED(hResult))
	{
		DEBUG_ERROR("Could not get pass desc");
		return (false);
	}

	hResult = m_rD10Renderer.GetDevice()->CreateInputLayout(_tpVetexDesc, _uiNumElements, tPassDesc.pIAInputSignature, tPassDesc.IAInputSignatureSize, &_prInputLayout);

	if (FAILED(hResult))
	{
		DEBUG_ERROR("Could not get pass desc");
		return (false);
	}

	return (true);
}

bool CD10Shader::CompileInputLayout(EInput* _epInputLayout, uint _uiInputCount, std::vector<D3D10_INPUT_ELEMENT_DESC>& _rInputElementDescs)
{
	uint uiByteOffset = 0;
	bool bReturn = true;

	for (uint i = 0; i < _uiInputCount; ++ i)
	{
		switch (_epInputLayout[i])
		{
		case  INPUT_POSITION3:
			{
				D3D10_INPUT_ELEMENT_DESC tInputDesc = { "POSITION" , 0 , DXGI_FORMAT_R32G32B32_FLOAT , 0 , uiByteOffset , D3D10_INPUT_PER_VERTEX_DATA , 0 };
				_rInputElementDescs.push_back(tInputDesc);

				uiByteOffset += sizeof(float) * 3;
			}
			break;

		case  INPUT_POSITION2:
			{
				D3D10_INPUT_ELEMENT_DESC tInputDesc = { "POSITION_TWO" , 0, DXGI_FORMAT_R32G32_FLOAT, 0, uiByteOffset, D3D10_INPUT_PER_VERTEX_DATA, 0};
				_rInputElementDescs.push_back(tInputDesc);

				uiByteOffset += sizeof(float) * 2;
			}
			break;

		case  INPUT_NORMAL3:
			{
				D3D10_INPUT_ELEMENT_DESC tInputDesc = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, uiByteOffset, D3D10_INPUT_PER_VERTEX_DATA, 0};
				_rInputElementDescs.push_back(tInputDesc);

				uiByteOffset += sizeof(float) * 3;
			}
			break;

		case INPUT_NORMAL2:
			{
				D3D10_INPUT_ELEMENT_DESC tInputDesc = { "NORMAL_TWO", 0, DXGI_FORMAT_R32G32_FLOAT, 0, uiByteOffset, D3D10_INPUT_PER_VERTEX_DATA, 0};
				_rInputElementDescs.push_back(tInputDesc);

				uiByteOffset += sizeof(float) * 2;
			}
			break;

		case INPUT_TEXT_CORDS:
			{
				D3D10_INPUT_ELEMENT_DESC tInputDesc = { "TEXTURE_CORDS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, uiByteOffset, D3D10_INPUT_PER_VERTEX_DATA, 0};
				_rInputElementDescs.push_back(tInputDesc);

				uiByteOffset += sizeof(float) * 2;
			}
			break;
		}
	}

	return (bReturn);
}

bool CD10Shader::LoadIncludes()
{
	if ((m_uiIncludes & INCLUDE_TRANSFORM) > 0)
	{
		VALIDATE(RegisterVariable("g_xWorld"));
		VALIDATE(RegisterVariable("g_xView"));
		VALIDATE(RegisterVariable("g_xProjection"));
		VALIDATE(RegisterVariable("g_xWorldViewProjection"));
	}

	if ((m_uiIncludes & INCLUDE_LIGHTING) > 0)
	{
		VALIDATE(RegisterVariable("g_tLights"));
		VALIDATE(RegisterVariable("g_tMaterials"));
		VALIDATE(RegisterVariable("g_iNumLights"));
		VALIDATE(RegisterVariable("g_iNumMaterials"));
		VALIDATE(RegisterVariable("g_vCameraPosition"));
	}

	if ((m_uiIncludes & INCLUDE_TEXTURING) > 0)
	{
		VALIDATE(RegisterVariable("g_TextureResources"));
		VALIDATE(RegisterVariable("g_iTextureCount"));
		VALIDATE(RegisterVariable("g_iTextureTypes"));
	}

	return (true);
}