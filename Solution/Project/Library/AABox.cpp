
//
//  File Name   :   AABox.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "AABox.h"

// Local Includes
#include "Framework/Debug.h"

// Library Includes

// Static Initialisers

// Implementation

TAABox::TAABox()
: m_fHalfWidth(0)
{
	FW_MEMZERO(m_fSideOffset, sizeof(float)* MAX_SIDE);
}

TAABox::~TAABox()
{
	 // Empty
}

void TAABox::SetBounds(const TVector3& _vCenterPosition, float _fHalfWidth, float _fHalfHeight)
{
	c_float kfCenterPosX = _vCenterPosition.x;
	c_float kfCenterPosY = _vCenterPosition.y;
	c_float kfCenterPosZ = _vCenterPosition.z;

	m_fSideOffset[SIDE_RIGHT]	= _vCenterPosition.x + _fHalfWidth;
	m_fSideOffset[SIDE_TOP]		= _vCenterPosition.y + _fHalfHeight;
	m_fSideOffset[SIDE_FRONT]	= _vCenterPosition.z + _fHalfWidth;

	m_fSideOffset[SIDE_LEFT]	= _vCenterPosition.x - _fHalfWidth;
	m_fSideOffset[SIDE_BOTTOM]	= _vCenterPosition.y - _fHalfHeight;
	m_fSideOffset[SIDE_BACK]	= _vCenterPosition.z - _fHalfWidth;
}

bool TAABox::IsWithinBounds(TVector3& _vrPoint) const
{
	if (_vrPoint.x > m_fSideOffset[SIDE_LEFT]	&& _vrPoint.x < m_fSideOffset[SIDE_RIGHT]	&&
		_vrPoint.y > m_fSideOffset[SIDE_BOTTOM] && _vrPoint.y < m_fSideOffset[SIDE_TOP]		&&
		_vrPoint.z > m_fSideOffset[SIDE_BACK]	&& _vrPoint.z < m_fSideOffset[SIDE_FRONT])
	{
		return (true);
	}

	return (false);
}

bool TAABox::IsWithinBounds(float _fX, float _fY, float _fZ) const
{
	if (_fX > m_fSideOffset[SIDE_LEFT]	 && _fX < m_fSideOffset[SIDE_RIGHT] &&
		_fY > m_fSideOffset[SIDE_BOTTOM] && _fY < m_fSideOffset[SIDE_TOP]	&&
		_fZ > m_fSideOffset[SIDE_BACK]	 && _fZ < m_fSideOffset[SIDE_FRONT])
	{
		return (true);
	}

	return (false);
}