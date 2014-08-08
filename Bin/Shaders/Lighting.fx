//
//  Bryce Booth
//  Auckland
//  New Zealand
//
//  File Name   :   LightHelper.fx
//  Description :   ----------------------
//
//  Author      :   Bryce Booth
//  Mail        :   bryce.booth@mediadesign.school.nz
//


#ifndef LIGHT_HELPER
#define LIGHT_HELPER


// Local Includes


// Defines
#define MAX_LIGHTS		8
#define MAX_MATERIALS   	2
#define PARALLEL_LIGHT	0
#define POINT_LIGHT		1
#define SPOT_LIGHT		2


// Types
struct TLight
{
	float3 vPosition;
	float3 vDirection;
	float4 vDiffuse;
	float4 vAmbient;
	float4 vSpecular;
	float  fAttenuation0;
	float  fAttenuation1;
	float  fAttenuation2;
	float  fRange;
	float  fTheta;
	float  fPhi;
	float  fFalloff;
	int    iType;
};


struct TMaterial
{
	float4 vDiffuse;
	float4 vAmbient;
	float4 vSpecular;
	float  fSpecularPower;
};


struct TSurfaceInfo
{
	float4 vWPosition;
    float3 vWNormal;
    float4 vDiffuse;
    float4 vSpecular;
	float4 vAmbient;
	float fA;
};


// Globals
cbuffer cbLightBuffer
{
	TLight		g_tLights[MAX_LIGHTS];
	TMaterial	g_tMaterials[MAX_MATERIALS];
	int			g_iNumLights;
	int			g_iNumMaterials;


	float3 g_vCameraPosition;
};


// Prototypes


// [Implementation]


inline float4
CalculatePhongLighting(TSurfaceInfo _tSurface, float4 LColor, float4 AColor, float4 _v4Specular, float3 N, float3 L, float3 V, float3 R)
{
	float4 Ia = _tSurface.vAmbient * AColor;
	float4 Id = _tSurface.vDiffuse * saturate(dot(N, L));
	float4 Is = _v4Specular * pow( saturate(dot(R , V)), _tSurface.fA);
	

	return (Ia + (Id + Is) * LColor);
}


inline float4 
ParallelLight(TSurfaceInfo _tSurfaceInfo, TLight _tLight, float4x4 _xWorld)
{
	//calculate lighting vectors - renormalize vectors
	_tSurfaceInfo.vWNormal = normalize(_tSurfaceInfo.vWNormal);
	float3 V = normalize( g_vCameraPosition - (float3)_tSurfaceInfo.vWPosition );
	
	
	//DONOT USE -light.dir since the reflection returns a ray from the surface
	float3 R = reflect( _tLight.vDirection, _tSurfaceInfo.vWNormal);


	float4 vLitColor = CalculatePhongLighting( _tSurfaceInfo, _tLight.vDiffuse, _tLight.vAmbient, _tLight.vSpecular, _tSurfaceInfo.vWNormal, -_tLight.vDirection, V, R);


	return (vLitColor);
}


inline float3
PointLight(TSurfaceInfo _tSurfaceInfo, TLight _tLight, float4x4 _xWorld)
{
	float3 vLitColour = float3(0.0f, 0.0f, 0.0f);
	

	// The vector from the surface to the light.
	float3 vLightVector = _tLight.vPosition - _tSurfaceInfo.vWPosition;
	

	// The distance from surface to light.
	float fDistance = length(vLightVector);
	

	if(fDistance <= _tLight.fRange)
	{
		// Normalize the light vector.
		vLightVector /= fDistance; 
	

		// Add the ambient light term.
		vLitColour += _tSurfaceInfo.vDiffuse * _tLight.vAmbient;	
	

		// Add diffuse and specular term, provided the surface is in 
		// the line of site of the light.
		float fDiffuseFactor = dot(vLightVector, _tSurfaceInfo.vWNormal);


		[branch]
		if(fDiffuseFactor > 0.0f)
		{
			float  fSpecularPower	= max(_tSurfaceInfo.vSpecular.a, 1.0f);
			float3 vToEye			= normalize(mul(g_vCameraPosition, (float3x3)_xWorld) - _tSurfaceInfo.vWPosition);
			float3 vReflection		= reflect(-vLightVector, _tSurfaceInfo.vWNormal);
			float  fSpecularFactor	= pow(max(dot(vReflection, vToEye), 0.0f), fSpecularPower);
	
			// diffuse and specular terms
			vLitColour += fDiffuseFactor * _tSurfaceInfo.vDiffuse * _tLight.vDiffuse;
			vLitColour += fSpecularFactor * _tSurfaceInfo.vSpecular * _tLight.vSpecular;
		}
	
		// attenuate
		vLitColour /= dot(float3(_tLight.fAttenuation0, _tLight.fAttenuation1, _tLight.fAttenuation2), float3(1.0f, fDistance, fDistance * fDistance));
	}
	

	return (vLitColour);
}


inline float3
Spotlight(TSurfaceInfo _tSurfaceInfo, TLight _tLight, float4x4 _xWorld)
{
	float3 vLitColour = PointLight(_tSurfaceInfo, _tLight, _xWorld);
	

	// The vector from the surface to the light.
	float3 vLightVector = normalize(_tLight.vPosition - _tSurfaceInfo.vWPosition);
	

	// Scale color by spotlight factor.
	float fScale = pow(max(dot(-vLightVector, _tLight.vDirection), 0.0f), _tLight.fFalloff);
	vLitColour *= fScale;


	return (vLitColour);
}


inline float3
CalculateAllLighting(TSurfaceInfo _tServiceInfo)
{
	float3 vTotalLight = float3(0.0f, 0.0f, 0.0f);


	for (int i = 0; i < g_iNumLights; ++ i)
	{
		[branch]
		if (g_tLights[i].iType == PARALLEL_LIGHT) 
		{
			vTotalLight += ParallelLight(_tServiceInfo, g_tLights[i], g_xWorld);
		}
		else if(g_tLights[i].iType == POINT_LIGHT) 
		{
			vTotalLight += PointLight(_tServiceInfo, g_tLights[i], g_xWorld);
		}
		else if(g_tLights[i].iType == SPOT_LIGHT) 
		{
			vTotalLight += Spotlight(_tServiceInfo, g_tLights[i], g_xWorld);
		}
	}


	return (vTotalLight);
}


#endif // LIGHT_HELPER