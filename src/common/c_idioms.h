#pragma once

//***************************************************************************
// Basic Idioms
//***************************************************************************

// assertion.
#define E_ASSERT(x)		assert(x)

// unreferenced parameter.
#define E_UNREF_PARAM( x )				( (void)(x) )
#define E_UNREF_PARAM2( x, y )			( E_UNREF_PARAM( x ), E_UNREF_PARAM( y ) )
#define E_UNREF_PARAM3( x, y, z )		( E_UNREF_PARAM( x ), E_UNREF_PARAM( y ), E_UNREF_PARAM( z ) )
#define E_UNREF_PARAM4( x, y, z, w )	( E_UNREF_PARAM( x ), E_UNREF_PARAM( y ), E_UNREF_PARAM( z ), E_UNREF_PARAM( w ) )

// define NULL value.
#ifndef NULL
# ifdef __cplusplus
#  define NULL    0
# else
#  define NULL    ((void *)0)
# endif
#endif

// bitflag helpers.
#define BIT( x )					(1 << (x))
#define HAS_FLAGS( x, flag )		(((x) & (flag)) == (flag))

// pointer helpers.
#define PTR_OFFSET_FROM_UINT( ui )	((physx::PxU8*)(size_t)(ui))

// invoke a member function by pointer.
#define CALL_MEMBER_FN( object, ptrToMember )	((object).*(ptrToMember))

// the number of elements in an array.
#define E_ARRAY_COUNT( TheType, TheArray )	(sizeof(TheArray) / sizeof(TheType))

// pointer-to-implementation.
#define E_IMPL(name)		\
	name##_impl* const	mImpl;	\
	name##_impl&		m;
#define E_IMPL_NAME(name)		\
	name##_impl
#define E_IMPL_NEW(name, ...)		mImpl(E_NEW(name##_impl)(__VA_ARGS__)), m(*mImpl)
#define E_IMPL_DEL(name)			delete mImpl

// no copy.
#define E_NO_COPY(name)			\
	name( const name& );		\
	name&		operator = ( const name& );

// swap.
template <typename _T>
E_INLINE void	Swap( _T& a, _T& b )
{
	_T c = a;
	a = b;
	b = c;
}
