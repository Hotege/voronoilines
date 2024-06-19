
#include "geometry.h"
#include <cmath>
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
    double distance(const vertex& v0, const vertex& v1)
    {
        double dx = v0.x - v1.x, dy = v0.y - v1.y;
        return sqrt(dx * dx + dy * dy);
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
    const segment& edge::get_vertical() const
    {
        return vertical;
    }
    const bool& edge::get_vertical_valid() const
    {
        return valid;
    }
    void edge::set_vertical(const segment& s)
    {
        vertical = s;
    }
    void edge::set_vertical_valid(bool v)
    {
        valid = v;
    }
} // namespace vl
