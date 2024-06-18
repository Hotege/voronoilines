
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
} // namespace vl
