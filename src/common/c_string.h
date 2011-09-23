#pragma once

//***************************************************************************
// Declarations
//***************************************************************************

// import string type.
#include <string>
using std::string;
using std::wstring;
extern const string		SNULL;
extern const wstring	WSNULL;

// useful typedefs.
typedef vector<string>				StringList;


//***************************************************************************
// Various string utility functions.
//***************************************************************************
#include <cstring>

// case-insensitive string search.
char*			stristr( const char *pszText, const char *pszSub );

E_INLINE bool	StringContains( const string& sHaystack, const string& sNeedle )	{ return sHaystack.find(sNeedle) != string::npos; }

extern bool		StringEqual( const char* s1, const char* s2 );
extern bool		StringEqual( const char* s1, const char* s2, bool bIgnoreCase );

E_INLINE bool	IsWhitespace( char c )							{	return isspace( c ) ? true : false;				}
extern bool		IsWhitespace( const string& sStr );

E_INLINE bool	IsEmpty( const char* str )						{	return ( (str == NULL) || (*str == '\0') );		}

// "foo/bar/baz.jpg" => "foo/bar/"
extern string	Path_GetDirectory( const string& sPath );
// "foo/bar/baz.jpg" => "baz.jpg"
extern string	Path_GetFilename( const string& sPath );
// "foo/bar/baz.jpg" => "baz"
extern string	Path_GetFilenameName( const string& sPath );
// "foo/bar/baz.jpg" => ".jpg"
extern string	Path_GetFilenameExtension( const string& sPath );
// "foo/bar/baz.jpg" => "foo/bar/baz"
extern string	Path_StripExtension( const string& sPath );

extern void		TrimFast( string& sStr );
extern string	Trim( const string& sStr );
extern bool		StartsWith( const string& sStr, const string& sWith, bool bIgnoreCase );
extern bool		StartsWith( const string& sStr, const string& sWith );
extern bool		EndsWith( const string& sStr, const string& sWith, bool bIgnoreCase );
extern bool		EndsWith( const string& sStr, const string& sWith );

extern string	Lowercase( const string& sStr );
extern string	Uppercase( const string& sStr );

// wildcard match.  E.g:  WildcardMatch("ro?k.*", "rock.jpg") ==> true
extern bool		EqualsWildcard( const char* pWild, const char* pString, bool bIgnoreCase = false );
extern bool		EqualsWildcard( const string& sWild, const string& sString, bool bIgnoreCase = false );

// wildcard match against a list of strings.
E_INLINE void	MatchWildcard( StringList& vOutMatches,
							  const string& sWild,
							  const StringList& vStrings,
							  bool bIgnoreCase = false )
{
	for ( size_t i(0); ( i < vStrings.size() ); ++i )
	{
		if ( EqualsWildcard( sWild, vStrings[i], bIgnoreCase ) )
			vOutMatches.push_back( vStrings[i] );
	}
}


extern PxU32	SplitBy( StringList&		vResults,
						// 'sSplitting' is being split by any character in 'sByChars.'
						const string&		sSplitting,
						const string&		sByChars,
						PxU32				uiStartAt = 0,
						// whether we should strip whitespace from the sides of entries.
						bool				bTrimWhitespace = false,
						// when 'bKeepSplitters' is true, each of those characters
						// will be added to the results. For example, 
						// splitting "foo/bar" would result in:
						//  [0] "foo"
						//  [1] "/"
						//  [2] "bar"
						bool				bKeepSplitters = false,
						bool				bKeepIfEmpty = false );

// searches a table of strings for the specified string.  For example, if you have:
//
//	const char* pImageExtensions = {
//		".tga", ".png", ".jpg", ".jpeg", ".exr", ".bmp", ".tif"
//	};
//
// ... then FindStringInList(pImageExtensions, ".jpg") would return 2.
//
// When no string is found, -1 is returned.
//
E_INLINE PxI32	FindStringInList(
								 // the string to find.
								 const char*	sFind,
								 // the table of strings.  It must be terminated with a NULL, e.g:
								 //
								 //		const char* kStrings[] = { "foo", "bar", NULL };
								 //
								 const char**	pStrings,
								 // whether we're ignoring case.
								 bool			bIgnoreCase = false )
{
	if ( bIgnoreCase )
	{
		for ( PxU32 i = 0; pStrings[i]; ++i )
			if ( stricmp( pStrings[i], sFind ) == 0 )
				return (PxI32)i;
	}
	else
	{
		for ( PxU32 i = 0; pStrings[i]; ++i )
			if ( strcmp( pStrings[i], sFind ) == 0 )
				return (PxI32)i;
	}
	return -1;
}


//***************************************************************************
// String Conversion
//***************************************************************************
string			ItoA( PxI32 val, PxU32 radix = 10 );