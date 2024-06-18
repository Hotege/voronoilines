
#include "voronoi.h"
#include <CDT.h>

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
            triangle T;
            T.vertices[0] = (size_t)t.vertices[0];
            T.vertices[1] = (size_t)t.vertices[1];
            T.vertices[2] = (size_t)t.vertices[2];
            res.push_back(T);
        }
        return res;
    }
} // namespace vl
