#include "common_afx.h"

// forward declarations.
int wildcmp(const char *wild, const char *string);

//***************************************************************************
// Definitions
//***************************************************************************
const string	SNULL("");
const wstring	WSNULL(L"");

//---------------------------------------------------------------------------
E_INLINE bool
StringEqual( const char* s1, const char* s2 )
{
	return (strcmp(s1, s2) == 0);
}

//---------------------------------------------------------------------------
E_INLINE bool
StringEqual( const char* s1, const char* s2, bool bIgnoreCase )
{
	if ( bIgnoreCase )
		return (stricmp( s1, s2 ) == 0);
	else
		return (strcmp( s1, s2 ) == 0);
}

//---------------------------------------------------------------------------
bool
IsWhitespace( const string& sStr )
{
	size_t uiEnd( sStr.length() );
	for ( size_t ui(0); ui < uiEnd; ++ui )
	{
		char c = sStr[ui];
		if ( !IsWhitespace( c ) )
			return false;
	}
	return true;
}

//---------------------------------------------------------------------------
string
Path_GetDirectory( const string& sPath )
{
	size_t uiSlashPos = sPath.find_last_of("/\\");
	if ( uiSlashPos == string::npos )
		return SNULL;

	return sPath.substr( 0, (uiSlashPos + 1) );
}

//---------------------------------------------------------------------------
string
Path_GetFilename( const string& sPath )
{
	size_t uiSlashPos = sPath.find_last_of("/\\");
	if ( uiSlashPos == string::npos )
		return sPath;

	return sPath.substr( uiSlashPos + 1 );
}

//---------------------------------------------------------------------------
string
Path_GetFilenameName( const string& sPath )
{
	string sFilename( Path_GetFilename( sPath ) );

	size_t uiPeriodPos = sFilename.find_last_of(".");
	if ( uiPeriodPos == string::npos )
		return sPath;

	return sPath.substr( 0, uiPeriodPos );
}

//---------------------------------------------------------------------------
string
Path_GetFilenameExtension( const string& sPath )
{
	size_t uiPeriodPos = sPath.find_last_of(".");
	if ( uiPeriodPos == string::npos )
		return SNULL;

	return sPath.substr( uiPeriodPos );
}

//---------------------------------------------------------------------------
string
Path_StripExtension( const string& sPath )
{
	size_t uiPeriodPos = sPath.find_last_of(".");
	if ( uiPeriodPos == string::npos )
		return sPath;

	return sPath.substr( 0, uiPeriodPos );
}

//---------------------------------------------------------------------------
void
TrimFast( string& sStr )
{
	size_t uiSubBegin( 0 );
	size_t uiSubEnd( sStr.size() );

	// trim left.
	for ( ; (uiSubBegin <= uiSubEnd) && IsWhitespace(sStr[uiSubBegin]); ++uiSubBegin )
	{
	}

	// trim right.
	for ( ; (uiSubEnd > uiSubBegin) && IsWhitespace(sStr[uiSubEnd - 1]); --uiSubEnd )
	{
	}

	sStr = sStr.substr( uiSubBegin, (uiSubEnd - uiSubBegin) );
}

//---------------------------------------------------------------------------
string
Trim( const string& sStr )
{
	string sResult(sStr);
	TrimFast(sResult);
	return sResult;
}

//---------------------------------------------------------------------------
PxU32
SplitBy( StringList&		vResults,
		const string&		sSplitting,
		const string&		sByChars,
		PxU32				uiStartAt,
		bool				bTrimWhitespace,
		bool				bKeepSplitters,
		bool				bKeepIfEmpty )
{
	size_t uiPrevResultsCount(vResults.size());

	// preallocate some room.
	vResults.reserve( 16 );

	size_t uiEnd = sSplitting.length();
	size_t uiPrev = uiStartAt;
	size_t uiPos = sSplitting.find_first_of( sByChars.c_str(), uiStartAt );
	while ( uiPos != string::npos )
	{
		vResults.push_back( sSplitting.substr( uiPrev, (uiPos - uiPrev) ) );
		if ( bTrimWhitespace )
			TrimFast( vResults.back() );

		// discard empty results, if desired.
		if ( !bKeepIfEmpty && (vResults.back().empty() || IsWhitespace(vResults.back())) )
			vResults.pop_back();

		// append the splitter character as its own result, if desired.
		if ( bKeepSplitters )
			vResults.push_back( sSplitting.substr( uiPos, 1 ) );

		// track the previous splitter position and search for the next one.
		uiPrev = (uiPos + 1);
		uiPos = sSplitting.find_first_of( sByChars.c_str(), (uiPos + 1) );
	}
	if ( uiPrev < uiEnd )
	{
		// push any remainder.
		vResults.push_back( sSplitting.substr( uiPrev ) );
		if ( bTrimWhitespace )
			TrimFast( vResults.back() );

		// discard empty results, if desired.
		if ( !bKeepIfEmpty && (vResults.back().empty() || IsWhitespace(vResults.back())) )
			vResults.pop_back();
	}

	if ( vResults.size() <= uiPrevResultsCount )
		return 0;
	else
		return (PxU32)(vResults.size() - uiPrevResultsCount);
}

//---------------------------------------------------------------------------
bool
StartsWith( const string& sStr, const string& sWith, bool bIgnoreCase )
{
	if ( sWith.size() > sStr.size() )
		return false;

	if ( bIgnoreCase )
	{
		if ( strnicmp( sStr.data(), sWith.data(), sWith.size() ) )
			return false;
	}
	else
	{
		if ( strncmp( sStr.data(), sWith.data(), sWith.size() ) )
			return false;
	}

	return true;
}

//---------------------------------------------------------------------------
bool
StartsWith( const string& sStr, const string& sWith )
{
	if ( sWith.size() > sStr.size() )
		return false;

	if ( strncmp( sStr.data(), sWith.data(), sWith.size() ) )
		return false;

	return true;
}


//---------------------------------------------------------------------------
bool
EndsWith( const string& sStr, const string& sWith, bool bIgnoreCase )
{
	if ( sWith.size() > sStr.size() )
		return false;

	if ( bIgnoreCase )
	{
		if ( strnicmp( (sStr.data() + sStr.size() - sWith.size()), sWith.data(), sWith.size() ) )
			return false;
	}
	else
	{
		if ( strncmp( (sStr.data() + sStr.size() - sWith.size()), sWith.data(), sWith.size() ) )
			return false;
	}

	return true;
}

//---------------------------------------------------------------------------
bool
EndsWith( const string& sStr, const string& sWith )
{
	if ( sWith.size() > sStr.size() )
		return false;

	if ( strncmp( (sStr.data() + sStr.size() - sWith.size()), sWith.data(), sWith.size() ) )
		return false;

	return true;
}

//---------------------------------------------------------------------------
bool
EqualsWildcard( const char* pWild, const char* pString, bool bIgnoreCase )
{
	E_ASSERT( pWild != NULL && pString != NULL );
	if ( !pWild || !pString )
		return false;

	// if the wildcard is empty, then always match.  (It's the behavior
	// of most filesearch programs.)
	if ( pWild[0] == '\0' )
		return true;

	// if it doesn't actually contain wildcard characters, then just do
	// a direct string comparison.
	if ( (strchr(pWild, '*') == NULL) && (strchr(pWild, '?') == NULL) )
	{
		if ( bIgnoreCase )
			return ( stricmp( pWild, pString ) == 0 );
		else
			return ( strcmp( pWild, pString ) == 0 );
	}

	// perform the wildcard search.
	return (wildcmp( pWild, pString ) > 0);
}

//---------------------------------------------------------------------------
bool
EqualsWildcard( const string& sWild, const string& sString, bool bIgnoreCase )
{
	return EqualsWildcard( sWild.c_str(), sString.c_str(), bIgnoreCase );
}

//---------------------------------------------------------------------------
string
Lowercase( const string& sStr )
{
	// TODO: make this UTF-8 safe.
	string sResult( sStr );
	for ( size_t i(0); ( i < sResult.size() ); ++i )
		sResult[i] = (char)tolower( sResult[i] );
	return sResult;
}

//---------------------------------------------------------------------------
string
Uppercase( const string& sStr )
{
	// TODO: make this UTF-8 safe.
	string sResult( sStr );
	for ( size_t i(0); ( i < sResult.size() ); ++i )
		sResult[i] = (char)toupper( sResult[i] );
	return sResult;
}


//***************************************************************************
// String Conversion
//***************************************************************************

//---------------------------------------------------------------------------
string
ItoA( PxI32 val, PxU32 radix )
{
	E_ASSERT( radix > 1 );

	char tmp[64];
	_itoa_s( val, tmp, 64, radix );
	return tmp;
}


//***************************************************************************
// Third-Party Source Code
//***************************************************************************

//---------------------------------------------------------------------------
// Source: http://bytes.com/topic/c/answers/129762-replacement-stristr
// Author:	Les Matheson
//			Integral Concepts, Inc.
//			http://www.ivsds.com
//---------------------------------------------------------------------------
char* stristr(const char *pszText, const char *pszSub)
{
	if (!pszText || !pszSub || !*pszText || !*pszSub)
		return 0;
	int nLen(int(strlen(pszSub)));
	const int test(toupper(*pszSub));
	while (*pszText)
	{
		if (toupper(*pszText)==test)
		{
			if (strnicmp(pszText,pszSub,nLen)==0)
				return (char*)pszText;
		}
		pszText++;
	}
	return 0;
}

//---------------------------------------------------------------------------
// Source:  http://www.codeproject.com/KB/string/wildcmp.aspx
// Author:	Jack Handy.
//---------------------------------------------------------------------------
int wildcmp(const char *wild, const char *string) {

  const char *cp = NULL, *mp = NULL;

  while ((*string) && (*wild != '*')) {
    if ((*wild != *string) && (*wild != '?')) {
      return 0;
    }
    wild++;
    string++;
  }

  while (*string) {
    if (*wild == '*') {
      if (!*++wild) {
        return 1;
      }
      mp = wild;
      cp = string+1;
    } else if ((*wild == *string) || (*wild == '?')) {
      wild++;
      string++;
    } else {
      wild = mp;
      string = cp++;
    }
  }

  while (*wild == '*') {
    wild++;
  }
  return !*wild;
}
