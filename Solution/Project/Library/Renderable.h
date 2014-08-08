
//
//  File Name   :   Render.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __Render_H__
#define __Render_H__

// Dependent Includes

// Local Includes
#include "Defines/Types.h"

// Library Includes
#include <list>

// Prototypes

class CRenderable
{

	// Member Types
public:

	// Member Functions
public:

			 CRenderable();
	virtual ~CRenderable();

	virtual void	Render() = 0;

	virtual void	RenderingEnabled(bool _bEnabled);
	virtual void	RenderLayer(uchar _ucLayer);

	virtual uint	RenderOrderIndex()	const;
	virtual uchar	RenderLayer()		const;
	virtual bool	RenderingEnabled()	const;

	static std::list<CRenderable*>& GetRenderablesList();

protected:

private:

	CRenderable(const CRenderable& _krRender);
	CRenderable& operator = (const CRenderable& _krRender);

	// Member Variables
protected:

private:

	uint m_uiRenderOrderIndex;

	uchar m_ucRenderLayer;

	bool m_bRenderingEnabled;

	static std::list<CRenderable*> s_Renderables;

};

#endif //__Render_H__