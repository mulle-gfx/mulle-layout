//
// this wily header, should work for Objective-C and C as it doubles as
// our envelope header, but is included in C source
//
#ifndef mulle_layout_envelope_h__
#define mulle_layout_envelope_h__


#include "include.h"


#define MULLE__LAYOUT_VERSION   ((0 << 20) | (0 << 8) | 2)

#include "_mulle-layout-provide.h"

#ifdef __has_include
# if __has_include( "_mulle-layout-versioncheck.h")
#  include "_mulle-layout-versioncheck.h"
# endif
#endif

#endif
