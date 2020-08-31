// There should not be Dsrc nor Isrc in the include paths
// The two following lines are non-compliant
#include <path/to/Dsrc/include.h>
#include <path/to/Isrc/include.h>  

// Other mentions of Dsrc or Isrc like in the examples below are OK
// dir name contains Isrc or Dsrc but has other stuff before or after
#include <path/to/weirDsrc/include.h>
// Not an include directive, this is fine
int Dsrc = 3;