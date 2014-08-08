//
//  Bryce Booth
//  Auckland
//  New Zealand
//
//
//  File Name :   CLASSNAME.inl
//
//  Author    :   Bryce Booth
//  Mail      :   bryce.booth@mediadesign.school.nz
//


// Library Includes


// Local Includes


// Implementation


void
CFPCamera::SetRotationEnabled(bool _bEnabled)
{
	m_bRotationEnabled = _bEnabled;
}


void
CFPCamera::SetMovementEnabled(bool _bEnabled)
{
	m_bMovementEnabled = _bEnabled;
}


void
CFPCamera::SetSensitivity(float _fSensitivity)
{
	m_fSensitivity = _fSensitivity;
}


void
CFPCamera::SetMovementVelocity(float _fVelocity)
{
	m_fMovementVelocity = _fVelocity;
}