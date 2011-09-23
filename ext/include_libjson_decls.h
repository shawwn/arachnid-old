#pragma once

//***************************************************************************
// libjson forward declarations.
//***************************************************************************

// JSONNODE
#undef JSONNODE
#define JSONNODE void  /* so that JSONNODE* is void* */
typedef JSONNODE** JSONNODE_ITERATOR;

// JSONSTREAM
#ifdef JSON_STREAM
# undef JSONSTREAM
# define JSONSTREAM void
  //typedef void (*json_stream_callback_t)(JSONNODE *, void * identifier);
#endif
