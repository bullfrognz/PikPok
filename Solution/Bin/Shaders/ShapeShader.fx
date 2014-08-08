//
//  File Name   :   ShapeShader.fx
//  Description :   ----------------------
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

// Local Includes
#include "Transform.fx"
#include "Lighting.fx"
#include "Texturing.fx"

// Defines

// Globals
float4 g_fColour = float4(1.0f, 1.0f, 1.0f, 1.0f);

// Types
struct VS_INPUT
{
	float3 vPosition	: POSITION;
	float3 vNormal		: NORMAL;
};

struct VS_OUTPUT
{
	float4 vPosition	: SV_POSITION;
	float4 vWPosition	: WPOSITION;
	float3 vWNormal		: VNORMAL;
};

// Globals

// [Implementation]

VS_OUTPUT ProcessVertexShader(VS_INPUT _Input)
{
	VS_OUTPUT tOutput;
	tOutput.vPosition	= mul(float4(_Input.vPosition, 1.0f), g_xWorldViewProjection);
	tOutput.vWPosition	= mul(float4(_Input.vPosition, 1.0f), g_xWorld);
	tOutput.vWNormal	= mul(float4(_Input.vNormal, 0.0f), g_xWorld);

	return (tOutput);
}

float4 ProcessPixelShader(VS_OUTPUT _tInput) : SV_Target
{
	return (g_fColour);
}

technique10 Default
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, ProcessVertexShader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, ProcessPixelShader()));
	}
}
