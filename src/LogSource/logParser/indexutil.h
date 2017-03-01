#pragma once

#include "logging.h"
//Disable conditional expr constant warning
//Disable unreferenced formal parameter warning
#pragma warning(disable : 4127)
#pragma warning(disable : 4100)

//Also define the common Asserts to be the same thing.  
#undef IndexAssert
#define IndexAssert(expr) LogAssert(expr)
