
#if !defined(_VORONOI_LINES_POISSON_H_)
#define _VORONOI_LINES_POISSON_H_

#include "geometry.h"

namespace vl
{
    vertices poisson_distribute(double w, double h, double r, size_t k = 30);
} // namespace vl

#endif
