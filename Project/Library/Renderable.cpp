
//
//  File Name   :   Renderable.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "Renderable.h"

// Local Includes
#include "Framework/Debug.h"

// Library Includes

// Static Initialisers
std::list<CRenderable*> CRenderable::s_Renderables;

// Implementation

CRenderable::CRenderable()
: m_uiRenderOrderIndex(s_Renderables.size())
, m_ucRenderLayer(0)
, m_bRenderingEnabled(true)
{
	s_Renderables.push_back(this);
}

CRenderable::~CRenderable()
{
	s_Renderables.remove(this);
}

void CRenderable::RenderingEnabled(bool _bEnabled)
{
	m_bRenderingEnabled = _bEnabled;
}

void CRenderable::RenderLayer(uchar _ucLayer)
{
	m_ucRenderLayer = _ucLayer;
}

uint CRenderable::RenderOrderIndex() const
{
	return (m_uiRenderOrderIndex);
}

uchar CRenderable::RenderLayer() const
{
	return (m_ucRenderLayer);
}

bool CRenderable::RenderingEnabled() const
{
	return (m_bRenderingEnabled);
}

std::list<CRenderable*>& CRenderable::GetRenderablesList()
{
	return (s_Renderables);
}