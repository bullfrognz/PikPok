//
//  Bryce Booth
//  Auckland
//  New Zealand
//
//  File Name   :   TransformationHelper.fx
//  Description :   ----------------------
//
//  Author      :   Bryce Booth
//  Mail        :   bryce.booth@mediadesign.school.nz
//


#ifndef TRANSFORMATION_HELPER
#define TRANSFORMATION_HELPER


// Lobal Includes


// Globals


// Types


// [Implementation]


cbuffer cbPerObject
{
	float4x4 g_xWorld;
	float4x4 g_xView;
	float4x4 g_xProjection;
	float4x4 g_xWorldViewProjection;
}


#endif // TRANSFORMATION_HELPER