
#if !defined(_VORONOI_LINES_VORONOI_H_)
#define _VORONOI_LINES_VORONOI_H_

#include "geometry.h"

namespace vl
{
    triangles triangulate(const vertices& src, double w, double h);
} // namespace vl

#endif
