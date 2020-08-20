// Older version of the code with " in the include directive - should not be caught
#include "path/to/include.h"

// New version of the code has " - should be caught
#include "path/to/another/include.h"

// No quotes, this is fine
#include <path/to/yet/another/include.h>

