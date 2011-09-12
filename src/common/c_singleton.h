#pragma once

//===========================================================================
// because singletons can be allocated global-statically (before main()),
// then this might be required.
//===========================================================================
extern void			Sys_GlobalStaticInit();

//===========================================================================
// SINGLETON_DEF
//===========================================================================
#define SINGLETON_DEF( name )				\
											\
private:									\
											\
	/* the singleton instance. */			\
	static name*	sInstance;				\
	/* to prevent recursive construction. */\
	static bool		sRequested;				\
											\
	/* singletons can't be externally constructed, nor copied. */	\
	name();									\
	name( const name& );					\
	name&	operator = ( const name& );		\
											\
public:										\
											\
	static inline bool		IsReleased()	\
	{										\
		return ( sInstance == NULL );		\
	}										\
											\
	static inline bool		IsAllocated()	\
	{										\
		return ( sInstance != NULL );		\
	}										\
											\
	static inline name*		GetInstance()	\
	{										\
		if ( !sRequested )					\
		{									\
			sRequested = true;				\
			if ( sInstance == NULL )						\
			{												\
				Sys_GlobalStaticInit();						\
				/* allocate the singleton. */				\
				sInstance = E_NEW( name );					\
			}												\
		}									\
		return sInstance;					\
	}										\
											\
	static void				Release()		\
	{										\
		if ( sInstance != NULL )			\
		{									\
			/* release the singleton. */	\
			delete sInstance;				\
			sInstance = NULL;				\
			sRequested = false;				\
		}									\
	}

//===========================================================================
// SINGLETON_INIT
//===========================================================================
#define SINGLETON_INIT( name )				\
											\
name*	name::sInstance;					\
bool	name::sRequested;					
