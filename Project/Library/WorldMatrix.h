
//
//  File Name   :   WorldMatrix.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __WorldMatrix_H__
#define __WorldMatrix_H__

// Dependent Includes
#include "Library/Matrix.h"

// Local Includes

// Library Includes

// Prototypes

struct TWorldMatrix : public TMatrix
{

// Member Types

// Member Functions

	 TWorldMatrix();
	 TWorldMatrix(TMatrix& _xrMatrix);
	~TWorldMatrix();

	void		Scale(float _fScale);
	void		Scale(const TVector3& _kvrScale);
	void		Scale(float _fX, float _fY, float _fZ);
	void		RotateRadians(float _fPitch, float _fYaw, float _fRoll);
	void		LookAt(const TVector3& _kvrTargetPosition);

	void		PositionX(float _fX);
	void		PositionY(float _fY);
	void		PositionZ(float _fZ);
	void		Position(float _fX, float _fY, float _fZ);
	void		Position(const TVector3& _kv3rVector);

	void		TranslateX(float _fX);
	void		TranslateY(float _fY);
	void		TranslateZ(float _fZ);
	void		Translate(float _fX, float _fY, float _fZ);
	void		Translate(const TVector3& _kv3rVector);

	void		TranslateTowards(const TVector3& _ktrTarget, float _fDeltaSpeed);

	float		PositionX() const;
	float		PositionY() const;
	float		PositionZ() const;
	TVector3	Position () const;

	TVector3	OrientationX() const;
	TVector3	OrientationY() const;
	TVector3	OrientationZ() const;

// Member Variables

};

#endif //__WorldMatrix_H__