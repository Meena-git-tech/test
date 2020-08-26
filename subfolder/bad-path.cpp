// There should not be Dsrc nor Isrc in the include paths
// The two following lines are non-compliant
#include <path/to/weirDsrc/include.h>
#include <path/to/saIsrc/include.h>
#include <path/to/Dsrc/include.h>
// Not an include directive, this is fine