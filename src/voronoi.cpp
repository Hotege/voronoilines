
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
        for (auto it = cdt.triangles.begin(); it != cdt.triangles.end();)
        {
            auto& t = *it;
            auto p0 = src[t.vertices[0]];
            auto p1 = src[t.vertices[1]];
            auto p2 = src[t.vertices[2]];
            auto c = calc_center(p0, p1, p2);
            if (c.x < 0 || c.x > w || c.y < 0 || c.y > h)
                it = cdt.triangles.erase(it);
            else
                it++;
        }
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
} // namespace vl
