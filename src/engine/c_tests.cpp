#include "engine_afx.h"
#include "c_tests.h"

// 3rd-party library headers.
#include "include_libjson.h"
#include "include_lua.h"

//***************************************************************************
// Definitions
//***************************************************************************
SINGLETON_INIT( CTests );

//===========================================================================
// Tests - Private State
//===========================================================================
class CTests_impl
{
public:
	~CTests_impl();
	CTests_impl();

	bool	TestJson();

};

//---------------------------------------------------------------------------
CTests_impl::~CTests_impl()
{
}

//---------------------------------------------------------------------------
CTests_impl::CTests_impl()
{
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// JSON
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//---------------------------------------------------------------------------
bool
CTests_impl::TestJson()
{
	printf("\n=== TestJson #1 ===\n" );
	{
		JSONNODE* n(json_new(JSON_NODE));
		json_push_back( n, json_new_a("RootA", "Hello World") );
		JSONNODE* c( json_new(JSON_ARRAY) );
		json_set_name( c, "ArrayOfNumbers" );
		json_push_back( c, json_new_i("", 16) );
		json_push_back( c, json_new_i("", 42) );
		json_push_back( c, json_new_i("", 128) );
		json_push_back( n, c );
		json_char* jc = json_write_formatted( n );
		printf( "%s\n", jc );
		json_delete( n );
	}

	printf("\n=== TestJson #2 ===\n" );
	{
		struct SParseJSON
		{
			static void ParseJSON( JSONNODE* pParent )
			{
				JSONNODE_ITERATOR i = json_begin( pParent );
				while ( i != json_end( pParent ) )
				{
					JSONNODE* pChild( *i );

					// recursively call ourselves to dig deeper into the tree.
					char type = json_type( pChild );
					if ( type == JSON_ARRAY || type == JSON_NODE )
						ParseJSON( pChild );

					json_char* sName = json_name( pChild );

					if ( StringEqual( sName, "RootA" ) )
					{
						printf( "RootA: %s\n",
							json_as_string( pChild ) );
					}
					else if ( StringEqual( sName, "ChildA" ) )
					{
						printf( "ChildA: %s\n",
							json_as_string( pChild ) );
					}
					else if ( StringEqual( sName, "ChildB" ) )
					{
						printf( "ChildB: %d\n",
							json_as_int( pChild ) );
					}

					++i;
				}
			}
		};

		std::string json = "{\"RootA\":\"Value in parent node\",\"ChildNode\":{\"ChildA\":\"String Value\",\"ChildB\":42}}";
		JSONNODE* n = json_parse( json.c_str() );
		json_char* jc = json_write_formatted( n );
		printf( "%s\n", jc );
		SParseJSON::ParseJSON( n );
		json_delete( n );
	}

	return true;
}


//===========================================================================
// Tests
//===========================================================================

//---------------------------------------------------------------------------
CTests::CTests()
: E_IMPL_NEW(CTests)
{
}

//---------------------------------------------------------------------------
CTests::~CTests()
{
	E_IMPL_DEL(CTests);
}

//---------------------------------------------------------------------------
bool
CTests::RunAll()
{
	bool bSuccess = true;
	{
		// run each test.
		bSuccess &= m.TestJson();
	}
	return bSuccess;
}
