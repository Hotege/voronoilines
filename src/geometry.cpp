
#include "geometry.h"
#include <algorithm>

namespace vl
{
    vertex calc_center(const vertex& p0, const vertex& p1, const vertex& p2)
    {
        double x0 = p0.x, y0 = p0.y;
        double x1 = p1.x, y1 = p1.y;
        double x2 = p2.x, y2 = p2.y;
        double A1 = 2 * (x1 - x0);
        double B1 = 2 * (y1 - y0);
        double C1 = x1 * x1 + y1 * y1 - x0 * x0 - y0 * y0;
        double A2 = 2 * (x2 - x1);
        double B2 = 2 * (y2 - y1);
        double C2 = x2 * x2 + y2 * y2 - x1 * x1 - y1 * y1;
        double x = (C1 * B2 - C2 * B1) / (A1 * B2 - A2 * B1);
        double y = (A1 * C2 - A2 * C1) / (A1 * B2 - A2 * B1);
        return vertex{ x, y };
    }
    vertex calc_center(const vertices& points, const triangle& t)
    {
        auto p0 = points[t[0]], p1 = points[t[1]], p2 = points[t[2]];
        return calc_center(p0, p1, p2);
    }
    edge::edge(size_t _b, size_t _e) : b(_b), e(_e)
    {
        if (b > e)
        {
            size_t tmp = b;
            b = e;
            e = tmp;
        }
    }
    bool edge::operator==(const edge& E) const
    {
        return b == E.b && e == E.e;
    }
    size_t edge::from() const
    {
        return b;
    }
    size_t edge::to() const
    {
        return e;
    }
    voronoi::voronoi(const vertices& _points, const triangles& _facets)
    {
        points = _points;
        facets = _facets;
        p2f.resize(points.size());
        p2b.resize(points.size());
        f2b.resize(facets.size());
        for (size_t i = 0; i < facets.size(); i++)
        {
            auto& f = facets[i];
            auto i0 = f[0], i1 = f[1], i2 = f[2];
            p2f[i0].insert(i);
            p2f[i1].insert(i);
            p2f[i2].insert(i);
            size_t b0 = push_border_unique(edge(i0, i1));
            size_t b1 = push_border_unique(edge(i1, i2));
            size_t b2 = push_border_unique(edge(i2, i0));
            p2b[i0].insert(b0);
            p2b[i1].insert(b0);
            p2b[i1].insert(b1);
            p2b[i2].insert(b1);
            p2b[i2].insert(b2);
            p2b[i0].insert(b2);
            f2b[i].insert(b0);
            f2b[i].insert(b1);
            f2b[i].insert(b2);
        }
        b2f.resize(borders.size());
        for (size_t i = 0; i < f2b.size(); i++)
            for (auto& b : f2b[i])
                b2f[b].insert(i);
    }
    const vertices& voronoi::get_points() const
    {
        return points;
    }
    const triangles& voronoi::get_facets() const
    {
        return facets;
    }
    const edges& voronoi::get_borders() const
    {
        return borders;
    }
    size_t voronoi::push_border_unique(const edge& e)
    {
        auto it = std::find_if(borders.begin(), borders.end(),
            [&e](const edge& e_) { return e == e_; });
        if (it == borders.end())
        {
            borders.push_back(e);
            it = borders.end();
            it--;
        }
        return (size_t)std::distance(borders.begin(), it);
    }
} // namespace vl
