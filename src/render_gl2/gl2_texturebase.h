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
	virtual PxU32	GetMiplevelCount() =0;

	virtual PxU32	GetGLHandle() =0;
	virtual PxU32	GetGLTarget() =0;

	virtual void	DrawFullscreen( PxF32 fLeft, PxF32 fRight, PxF32 fBot, PxF32 fTop ) { E_UNREF_PARAM4( fLeft, fRight, fBot, fTop ); }
	void			DrawFullscreen()	{	DrawFullscreen( -1.0f, 1.0F, -1.0F, 1.0F );	}

protected:
	ETextureType	m_eType;
};