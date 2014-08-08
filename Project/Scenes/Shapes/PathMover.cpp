
//
//  File Name   :   PathMover.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "PathMover.h"

// Local Includes
#include "Framework/Debug.h"
#include "Framework/Clock.h"
#include "Library/Entity.h"

// Library Includes

// Static Initialisers

// Implementation

CPathMover::CPathMover(CEntity& _rEntity)
: m_rEntity(_rEntity)
, m_ePattern(INVALID_PATTERN)
, m_eTargetWaypoint(INVALID_WAYPOINT)
, m_fMoveSpeed(0.0f)
, m_fRadius(0.0f)
, m_fCircluarAngle(0.0f)
{
	// Empty
}

CPathMover::~CPathMover()
{
	// Empty
}

void CPathMover::Process()
{
	// Process pattern movement
	switch (m_ePattern)
	{
	case PATTERN_UP_DOWN:
		ProcessUpDownMovement();
		break;

	case PATTERN_LEFT_RIGHT:
		ProcessLeftRightMovement();
		break;

	case PATTERN_CIRCULAR:
		ProcessCircularMovement();
		break;

	case PATTERN_BOX:
		ProcessBoxMovement();
		break;

	default:
		DEBUG_WARNING("Unknown shape move pattern (%d)", m_ePattern);
		break;
	}
}

void CPathMover::CenterPosition(const TVector3& _ktrCenterPosition)
{
	m_tCenterPosition = _ktrCenterPosition;

	RegenerateWaypoints();
}

void CPathMover::Pattern(CPathMover::EPattern _ePattern)
{
	m_ePattern = _ePattern;

	RegenerateWaypoints();
}

void CPathMover::MoveSpeed(float _fSpeed)
{
	m_fMoveSpeed = _fSpeed;
}

void CPathMover::Radius(float _fRadius)
{
	m_fRadius = _fRadius;

	RegenerateWaypoints();
}

CPathMover::EPattern CPathMover::Pattern() const
{
	return (m_ePattern);
}

float CPathMover::MoveSpeed() const
{
	return (m_fMoveSpeed);
}

float CPathMover::Radius() const
{
	return (m_fRadius);
}

const TVector3& CPathMover::TargetWaypoint() const
{
	static TVector3 tInvalidWaypoint;

	if (m_eTargetWaypoint == INVALID_WAYPOINT)
		return (tInvalidWaypoint);

	return (m_tWaypoints[m_eTargetWaypoint]);
}

void CPathMover::ProcessUpDownMovement()
{
	// Move towards target waypoint
	m_rEntity.World().TranslateTowards(TargetWaypoint(), m_fMoveSpeed * CLOCK.Deltatick());

	// Check waypoint  has been reached
	if (m_rEntity.World().Position() != TargetWaypoint())
		return;

	// Set next waypoint
	switch (m_eTargetWaypoint)
	{
	case CPathMover::WAYPOINT_TOP:
		m_eTargetWaypoint = WAYPOINT_BOTTOM;
		break;

	case CPathMover::WAYPOINT_BOTTOM:
		m_eTargetWaypoint = WAYPOINT_TOP;
		break;

	default:
		DEBUG_ERROR("Invalid current target waypoint for pattern. Waypoint(%d)", m_ePattern, m_eTargetWaypoint);
		break;
	}
}

void CPathMover::ProcessLeftRightMovement()
{
	// Move towards target waypoint
	m_rEntity.World().TranslateTowards(TargetWaypoint(), m_fMoveSpeed * CLOCK.Deltatick());

	// Check waypoint  has been reached
	if (m_rEntity.World().Position() != TargetWaypoint())
		return;

	// Set next waypoint
	switch (m_eTargetWaypoint)
	{
	case CPathMover::WAYPOINT_LEFT:
		m_eTargetWaypoint = WAYPOINT_RIGHT;
		break;

	case CPathMover::WAYPOINT_RIGHT:
		m_eTargetWaypoint = WAYPOINT_LEFT;
		break;

	default:
		DEBUG_ERROR("Invalid current target waypoint for pattern. Pattern(%d) Waypoint(%d)", m_ePattern, m_eTargetWaypoint);
		break;
	}
}

void CPathMover::ProcessCircularMovement()
{
	if (m_eTargetWaypoint == WAYPOINT_CIRCULAR_START)
	{
		// Move towards target waypoint
		m_rEntity.World().TranslateTowards(TargetWaypoint(), m_fMoveSpeed * CLOCK.Deltatick());

		// Check waypoint  has been reached
		if (m_rEntity.World().Position() != TargetWaypoint())
			return;

		m_eTargetWaypoint = WAYPOINT_CIRCULAR;
	}

	else if (m_eTargetWaypoint == WAYPOINT_CIRCULAR)
	{
		// Move in a circle
		m_fCircluarAngle += m_fMoveSpeed * CLOCK.Deltatick();

		m_rEntity.World().Position(m_tCenterPosition.x + cos(m_fCircluarAngle) * m_fRadius,
								   0.0f,
								   m_tCenterPosition.z + sin(m_fCircluarAngle) * m_fRadius);
	}

	else
	{
		DEBUG_ERROR("Invalid current target waypoint for pattern. Pattern(%d) Waypoint(%d)", m_ePattern, m_eTargetWaypoint);
	}
}

void CPathMover::ProcessBoxMovement()
{
	// Move towards target waypoint
	m_rEntity.World().TranslateTowards(TargetWaypoint(), m_fMoveSpeed * CLOCK.Deltatick());

	// Check waypoint  has been reached
	if (m_rEntity.World().Position() != TargetWaypoint())
		return;

	// Set next waypoint
	switch (m_eTargetWaypoint)
	{
	case CPathMover::WAYPOINT_TOP_LEFT:
		m_eTargetWaypoint = WAYPOINT_TOP_RIGHT;
		break;

	case CPathMover::WAYPOINT_TOP_RIGHT:
		m_eTargetWaypoint = WAYPOINT_BOTTOM_RIGHT;
		break;

	case CPathMover::WAYPOINT_BOTTOM_RIGHT:
		m_eTargetWaypoint = WAYPOINT_BOTTOM_LEFT;
		break;

	case CPathMover::WAYPOINT_BOTTOM_LEFT:
		m_eTargetWaypoint = WAYPOINT_TOP_LEFT;
		break;

	default:
		DEBUG_ERROR("Invalid current target waypoint for pattern. Pattern(%d) Waypoint(%d)", m_ePattern, m_eTargetWaypoint);
		break;
	}
}

void CPathMover::RegenerateWaypoints()
{
	if (m_ePattern == INVALID_PATTERN)
		return;

	switch (m_ePattern)
	{
	case PATTERN_UP_DOWN:
		m_tWaypoints[WAYPOINT_TOP]		= m_tCenterPosition + TVector3(0.0f, 0.0f,  m_fRadius);
		m_tWaypoints[WAYPOINT_BOTTOM]	= m_tCenterPosition + TVector3(0.0f, 0.0f, -m_fRadius);

		m_eTargetWaypoint = WAYPOINT_TOP;
		break;

	case PATTERN_LEFT_RIGHT:
		m_tWaypoints[WAYPOINT_LEFT]		= m_tCenterPosition + TVector3(-m_fRadius, 0.0f, 0.0f);
		m_tWaypoints[WAYPOINT_RIGHT]	= m_tCenterPosition + TVector3( m_fRadius, 0.0f, 0.0f);

		m_eTargetWaypoint = WAYPOINT_LEFT;
		break;

	case PATTERN_CIRCULAR:
		m_tWaypoints[WAYPOINT_CIRCULAR_START] = m_tCenterPosition + TVector3(m_fRadius, 0.0f, 0.0f);

		m_eTargetWaypoint = WAYPOINT_CIRCULAR_START;
		m_fCircluarAngle  = 0.0f;
		break;

	case PATTERN_BOX:
		m_tWaypoints[WAYPOINT_TOP_LEFT]		= m_tCenterPosition + TVector3(-m_fRadius, 0.0f,  m_fRadius);
		m_tWaypoints[WAYPOINT_TOP_RIGHT]	= m_tCenterPosition + TVector3( m_fRadius, 0.0f,  m_fRadius);
		m_tWaypoints[WAYPOINT_BOTTOM_LEFT]	= m_tCenterPosition + TVector3(-m_fRadius, 0.0f, -m_fRadius);
		m_tWaypoints[WAYPOINT_BOTTOM_RIGHT]	= m_tCenterPosition + TVector3( m_fRadius, 0.0f, -m_fRadius);

		m_eTargetWaypoint = WAYPOINT_TOP_LEFT;
		break;

	default:
		DEBUG_WARNING("Unknown shape move pattern (%d)", m_ePattern);
		break;
	}
}