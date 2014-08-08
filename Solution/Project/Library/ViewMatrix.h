
//
//  File Name   :   ViewMatrix.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __ViewMatrix_H__
#define __ViewMatrix_H__

// Dependent Includes
#include "Library/Matrix.h"

// Local Includes
#include "Defines/Types.h"

// Library Includes

// Prototypes
struct TVector3;

struct TViewMatrix : public TMatrix
{

// Member Types

// Member Functions

	 TViewMatrix();
	~TViewMatrix();

	void LookAtLH(const TVector3& _krPosition, const TVector3& _krTarget);

	void GetLookAt(TVector3& _rLookAt) const;
	void GetRight(TVector3& _rRight) const;
	void GetUp(TVector3& _rUp) const;

	TVector3 GetLookAt() const;
	TVector3 GetRight() const;
	TVector3 GetUp() const;

// Member Variables

};

#endif //__ViewMatrix_H__