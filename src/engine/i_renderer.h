#pragma once

//***************************************************************************
// Declarations
//***************************************************************************

//===========================================================================
// Renderer
//===========================================================================
class E_API IRenderer
{
public:
	virtual ~IRenderer() { }
	IRenderer() { }
	virtual void			Release()		{ }

	virtual const char*		GetName()		{	return "null";	}

	virtual void			RegisterTexture( ResHandle hTex )		{	E_UNREF_PARAM( hTex );	}
	virtual void			UnregisterTexture( ResHandle hTex )		{	E_UNREF_PARAM( hTex );	}
};
