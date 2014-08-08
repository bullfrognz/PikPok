
//
//  File Name   :   Matrix.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __Matrix_H__
#define __Matrix_H__

// Dependent Includes

// Local Includes

// Library Includes

// Prototypes
struct TQuaternion;
struct TVector4;
struct TVector3;

struct TMatrix
{

// Member Types

// Member Functions

	 TMatrix();
	 TMatrix(const TQuaternion& _kxrQuaternion);
	~TMatrix();

	TMatrix& Identity();
	TMatrix& Inverse(TMatrix& _rTarget)  const;
	TMatrix& Invert();
	float	 Determinant() const;

	TVector4& TransformPoint(const TVector3& _kvrPoint, TVector4& _vrResult) const;

	TMatrix& operator += (const TMatrix& _kxrMatrix);
	TMatrix& operator -= (const TMatrix& _kxrMatrix);
	TMatrix& operator *= (const TMatrix& _kxrMatrix);
	TMatrix& operator  = (const TMatrix& _kxrMatrix);

	TMatrix operator + (const TMatrix& _kxrMatrix) const;
	TMatrix operator - (const TMatrix& _kxrMatrix) const;
	TMatrix operator * (const TMatrix& _kxrMatrix) const;

	bool operator == (const TMatrix& _kxrMatrix) const;
	bool operator != (const TMatrix& _kxrMatrix) const;

// Member Variables

	float values[4][4];

};

#endif //__Matrix_H__