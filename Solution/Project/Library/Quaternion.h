
//
//  File Name   :   Quaternion.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __Quaternion_H__
#define __Quaternion_H__

// Dependent Includes

// Prototypes

struct TQuaternion
{

// Member Types

// Member Functions

	 TQuaternion();
	 TQuaternion(const TQuaternion& _kvrRight);
	~TQuaternion();

	TQuaternion& RotateX(float _fX);
	TQuaternion& RotateY(float _fY);
	TQuaternion& RotateZ(float _fZ);
	TQuaternion& Rotate(float _fX, float _fY, float _fZ);

	TQuaternion& SetIdentity();
	TQuaternion& SetInvert();
	TQuaternion& SetConjugate();

	TQuaternion& GetInserse(TQuaternion& _vrRight);
	TQuaternion& GetConjugate(TQuaternion& _vrRight);
	TQuaternion  GetInserse();
	TQuaternion  GetConjugate();

	TQuaternion& operator += (const TQuaternion& _kvrRight);
	TQuaternion& operator -= (const TQuaternion& _kvrRight);
	TQuaternion& operator *= (const TQuaternion& _kvrRight);
	TQuaternion& operator *= (float _fRight);
	TQuaternion& operator /= (float _fRight);

	TQuaternion  operator + () const;
	TQuaternion  operator - () const;

	TQuaternion operator + (const TQuaternion& _kvrRight);
	TQuaternion operator - (const TQuaternion& _kvrRight);
	TQuaternion operator * (const TQuaternion& _kvrRight);
	TQuaternion operator * (float _fRight);
	TQuaternion operator / (float _fRight);

// Member Variables

	float x;
	float y;
	float z;
	float w;

};

#endif //__Quaternion_H__