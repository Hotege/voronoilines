
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
    class voronoi final
    {
    public:
        voronoi(const vertices& _points, const triangles& _facets,
            double w, double h);
        const vertices& get_points() const;
        const triangles& get_facets() const;
        const edges& get_borders() const;
    private:
        size_t push_border_unique(const edge& e);
        static vertices get_surroundings(const vertex& c0, const vertex& c,
            double w, double h);
    private:
        double width, height;
        vertices points;
        triangles facets;
        edges borders;
        std::vector<std::set<size_t>> p2f, p2b;
        std::vector<std::set<size_t>> b2f, f2b;
        std::vector<std::set<size_t>> p2n;
    };

    vertex calc_center(const vertex& p0, const vertex& p1, const vertex& p2);
    vertex calc_center(const vertices& points, const triangle& t);
    double distance(const vertex& v0, const vertex& v1);
} // namespace vl

#endif
