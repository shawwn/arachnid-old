#include "common_afx.h"

//***************************************************************************
// Definitions
//***************************************************************************
const string	SNULL("");
const wstring	WSNULL(L"");

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