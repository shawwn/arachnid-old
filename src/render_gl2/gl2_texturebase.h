#pragma once

//===========================================================================
// TextureBase
//===========================================================================
class GL2TextureBase
{
protected:
	GL2TextureBase( ETextureType eType );

public:
	virtual ~GL2TextureBase();

	ETextureType	GetType() const		{ return m_eType; }

	virtual PxU32	GetGLHandle() =0;
	virtual void	Bind( PxU32 uiTexUnit ) =0;

protected:
	ETextureType	m_eType;
};