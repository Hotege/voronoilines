
#if !defined(_VORONOI_LINES_VERTEX_H_)
#define _VORONOI_LINES_VERTEX_H_

#include <array>
#include <set>
#include <tuple>
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
    using triangle = array3;
    using triangles = std::vector<triangle>;
    using segment = std::pair<vertex, vertex>;
    class edge final
    {
    public:
        edge(size_t _b, size_t _e);
        bool operator==(const edge& E) const;
        size_t from() const;
        size_t to() const;
        const segment& get_vertical() const;
        const bool& get_vertical_valid() const;
        void set_vertical(const segment& s);
        void set_vertical_valid(bool v);
    private:
        size_t b;
        size_t e;
        segment vertical;
        bool valid = false;
    };
    using edges = std::vector<edge>;

    vertex calc_center(const vertex& p0, const vertex& p1, const vertex& p2);
    vertex calc_center(const vertices& points, const triangle& t);
    double distance(const vertex& v0, const vertex& v1);
} // namespace vl

#endif
