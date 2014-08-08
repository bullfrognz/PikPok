
//
//  File Name   :   Entity.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "Entity.h"

// Local Includes
#include "Framework/Debug.h"

// Library Includes

// Static Initialisers

// Implementation

CEntity::CEntity()
{
	// Empty
}

CEntity::~CEntity()
{
	// Empty
}

TWorldMatrix& CEntity::World()
{
	return (m_tWorld);
}