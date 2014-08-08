//
//  Bryce Booth
//  Auckland
//  New Zealand
//
//  File Name   :   TextureHelper.fx
//  Description :   ----------------------
//
//  Author      :   Bryce Booth
//  Mail        :   bryce.booth@mediadesign.school.nz
//


#ifndef __TextureHelper__
#define __TextureHelper__


// Local Includes


// Defines
#define MAX_TEXTURES 8


// Types
cbuffer cbTextureBuffer
{
	int g_iTextureTypes[MAX_TEXTURES];
	int g_iTextureCount;
};


// Globals
Texture2D g_TextureResources[MAX_TEXTURES];


// Sates
SamplerState linearSampler
{
	Filter = ANISOTROPIC;
	AddressU = WRAP;
	AddressV = WRAP;
	MaxAnisotropy = 4;
};


// [Implementation]


float3
CalculateAllTexturing(float2 _vTexCoords)
{
	float3 vTotalLighting = float3(0.0f, 0.0f, 0.0f);


	for (int i = 0; i < g_iTextureCount; ++ i)
	{
		vTotalLighting += g_TextureResources[i].Sample(linearSampler, _vTexCoords);
	}


	return (vTotalLighting);
}



#endif // __TextureHelper__