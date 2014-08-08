
//
//  File Name   :   PathMover.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __PathMover_H__
#define __PathMover_H__

// Dependent Includes
#include "Library/Processable.h"

// Local Includes
#include "Defines/Types.h"

// Library Includes
#include "Library/Vector3.h"

// Prototypes
class CEntity;

class CPathMover : public CProcessable
{

	// Member Types
public:

	enum EPattern
	{
		INVALID_PATTERN,

		PATTERN_UP_DOWN,
		PATTERN_LEFT_RIGHT,
		PATTERN_CIRCULAR,
		PATTERN_BOX,

		MAX_PATTERN
	};

	enum EWaypoint
	{
		INVALID_WAYPOINT		= -1,

		WAYPOINT_LEFT			= 0,
		WAYPOINT_RIGHT			= 1,

		WAYPOINT_TOP			= 0,
		WAYPOINT_BOTTOM			= 1,

		WAYPOINT_TOP_LEFT		= 0,
		WAYPOINT_TOP_RIGHT		= 1,
		WAYPOINT_BOTTOM_LEFT	= 2,
		WAYPOINT_BOTTOM_RIGHT	= 3,

		WAYPOINT_CIRCULAR_START	= 0,
		WAYPOINT_CIRCULAR		= 1,

		MAX_WAYPOINT			= 4
	};

	// Member Functions
public:

	 CPathMover(CEntity& _rEntity);
	~CPathMover();

	void Process();

	void CenterPosition(const TVector3& _ktrCenterPosition);
	void Pattern(EPattern _ePattern);
	void MoveSpeed(float _fSpeed);
	void Radius(float _fRadius);

	EPattern Pattern()	 const;
	float	 MoveSpeed() const;
	float	 Radius()	 const;
	const TVector3&	TargetWaypoint() const;

protected:

private:

	void ProcessUpDownMovement();
	void ProcessLeftRightMovement();
	void ProcessCircularMovement();
	void ProcessBoxMovement();

	void RegenerateWaypoints();

	CPathMover(const CPathMover& _krPathMover);
	CPathMover& operator = (const CPathMover& _krPathMover);

	// Member Variables
protected:

private:

	TVector3	m_tCenterPosition;
	TVector3	m_tWaypoints[4];
	
	CEntity&	m_rEntity;
	
	EPattern	m_ePattern;
	EWaypoint	m_eTargetWaypoint;

	float		m_fMoveSpeed;
	float		m_fRadius;
	float		m_fCircluarAngle;

};

#endif //__PathMover_H__