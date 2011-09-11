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
// Various Utility Functions
//***************************************************************************
#include <cstring>

E_INLINE bool	StringEqual( const char* s1, const char* s2 )	{	return (strcmp(s1, s2) == 0);		}
E_INLINE bool	IsWhitespace( char c )							{	return isspace( c ) ? true : false;	}

E_INLINE bool	StringContains( const string& sHaystack, const string& sNeedle )	{ return sHaystack.find(sNeedle) != string::npos; }

extern bool		IsWhitespace( const string& sStr );

extern void		TrimFast( string& sStr );
extern string	Trim( const string& sStr );
extern bool		StartsWith( const string& sStr, const string& sWith, bool bIgnoreCase );
extern bool		StartsWith( const string& sStr, const string& sWith );


extern PxU32	SplitBy( StringList&		vResults,
						const string&		sSplitting,
						const string&		sByChars,
						PxU32				uiStartAt = 0,
						bool				bTrimWhitespace = false,
						// 'sSplitting' is being split by any character in 'sByChars.'
						// When 'bKeepSplitters' is true, each of those characters
						// will be added to the results.
						bool				bKeepSplitters = false,
						bool				bKeepIfEmpty = false );
