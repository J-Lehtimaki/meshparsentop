#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <vector>

namespace feConstants{

// This is denominator for the total size of subtracted mesh
// (1000 means that one promille of mesh can have stress value
// that exceeds design stress limit)
const size_t NODE_COUNT_FEASIBILITY_RATIO = 1000;

}

#endif