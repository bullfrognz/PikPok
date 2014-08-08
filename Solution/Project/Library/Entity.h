
//
//  File Name   :   Entity.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __Entity_H__
#define __Entity_H__

// Dependent Includes

// Local Includes
#include "Library/WorldMatrix.h"
#include "Defines/Types.h"

// Library Includes

// Prototypes

class CEntity
{

	// Member Types
public:

	// Member Functions
public:

			 CEntity();
	virtual ~CEntity();

	TWorldMatrix& World();

protected:

private:

	CEntity(const CEntity& _krEntity);
	CEntity& operator = (const CEntity& _krEntity);

	// Member Variables
protected:

private:

	TWorldMatrix m_tWorld;

};

#endif //__Entity_H__