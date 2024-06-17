
#if !defined(_VORONOI_LINES_RANDOMIZE_H_)
#define _VORONOI_LINES_RANDOMIZE_H_

#include <cstdint>

namespace vl
{
    void randomize(unsigned int seed);
    size_t random_uint();
    double random_real();
} // namespace vl

#endif
