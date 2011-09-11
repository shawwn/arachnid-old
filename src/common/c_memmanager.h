#pragma once

//***************************************************************************
// Declarations
//***************************************************************************
#define E_NEW(type)				new type
#define E_DELETE( p )			(delete (p), (p)=NULL)
#define E_DELETE_ARRAY( p )		(delete [] (p), (p)=NULL)
