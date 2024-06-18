
#if !defined(_VORONOI_LINES_VERTEX_H_)
#define _VORONOI_LINES_VERTEX_H_

#include <array>
#include <vector>

namespace vl
{
    struct vertex
    {
        double x;
        double y;
    };
    using vertices = std::vector<vertex>;
    using array3 = std::array<size_t, 3>;
    struct triangle
    {
        array3 vertices;
    };
    using triangles = std::vector<triangle>;

    vertex calc_center(const vertex& p0, const vertex& p1, const vertex& p2);
} // namespace vl

#endif
