
#include "voronoi.h"
#include "geometry.h"
#include <CDT.h>

namespace vl
{
    triangles triangulate(const vertices& src, double w, double h)
    {
        triangles res;
        CDT::Triangulation<double> cdt;
        cdt.insertVertices(src.begin(), src.end(),
            [](const vertex& v) { return v.x; },
            [](const vertex& v) { return v.y; });
        cdt.eraseSuperTriangle();
        for (auto& t : cdt.triangles)
        {
            triangle T =
            {
                (size_t)t.vertices[0],
                (size_t)t.vertices[1],
                (size_t)t.vertices[2],
            };
            res.push_back(T);
        }
        return res;
    }
    voronoi::voronoi()
    {
    }
    voronoi::voronoi(const vertices& _points, const triangles& _facets,
        double w, double h) : width(w), height(h)
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
        for (size_t i = 0; i < borders.size(); i++)
        {
            auto fis = b2f[i];
            auto it = fis.begin();
            auto f0 = facets[*it];
            auto c0 = calc_center(points, f0);
            if (fis.size() == 2)
            {
                it++;
                auto f1 = facets[*it];
                auto c1 = calc_center(points, f1);
                if ((c0.x < 0 || c0.x > w || c0.y < 0 || c0.y > h) &&
                    (c1.x < 0 || c1.x > w || c1.y < 0 || c1.y > h))
                    continue;
                borders[i].set_vertical({ c0, c1 });
                borders[i].set_vertical_valid(true);
            }
            else if (fis.size() == 1)
            {
                if (c0.x < 0 || c0.x > w || c0.y < 0 || c0.y > h)
                    continue;
                auto i0 = borders[i].from(), i1 = borders[i].to();
                auto p0 = points[i0], p1 = points[i1];
                auto c = vertex{ (p0.x + p1.x) / 2, (p0.y + p1.y) / 2 };
                auto surroundings = get_surroundings(c0, c, w, h);
                if (surroundings.size() == 0)
                    continue;
                size_t pos = 0;
                double d = distance(surroundings[pos], c0);
                for (size_t j = 1; j < surroundings.size(); j++)
                {
                    double _d = distance(surroundings[j], c0);
                    if (_d < d)
                    {
                        pos = j;
                        d = _d;
                    }
                }
                borders[i].set_vertical({ c0, surroundings[pos] });
                borders[i].set_vertical_valid(true);
            }
        }
        p2v.resize(points.size());
        for (size_t i = 0; i < points.size(); i++)
        {
            for (auto& b : p2b[i])
            {
                if (borders[b].get_vertical_valid())
                {
                    auto& s = borders[b].get_vertical();
                    auto p0 = s.first, p1 = s.second;
                    bool b0 = false, b1 = false;
                    if (p0.x < 0 || p0.x > w || p0.y < 0 || p0.y > h)
                        b0 = true;
                    if (p1.x < 0 || p1.x > w || p1.y < 0 || p1.y > h)
                        b1 = true;
                    if (b0 && b1)
                        continue;
                    p2v[i].insert(b);
                }
            }
        }
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
    unique_indices voronoi::get_borders_by_point(size_t id) const
    {
        return p2b[id];
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
