
#include "geometry.h"
#include <cmath>
#include <algorithm>

namespace vl
{
    int compare(const vertex& v0, const vertex& v1, double prec)
    {
        if (abs(v1.x - v0.x) > prec)
            return -1;
        else if (abs(v0.x - v1.x) > prec)
            return 1;
        else
        {
            if (abs(v1.y - v0.y) > prec)
                return -1;
            else if (abs(v0.y - v1.y) > prec)
                return 1;
            else
                return 0;
        }
    }
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
    int sort(std::vector<vertex>& out, const std::vector<segment>& lines)
    {
        out.clear();
        if (lines.size() == 0)
            return 0;
        std::vector<bool> flags(lines.size(), true);
        out.push_back(lines[0].first);
        out.push_back(lines[0].second);
        flags[0] = false;
        while (true)
        {
            vertex v0 = out[0], v1 = out[out.size() - 1];
            bool updated = false;
            for (size_t i = 0; i < lines.size(); i++)
            {
                if (!flags[i])
                    continue;
                auto v2 = lines[i].first, v3 = lines[i].second;
                if (0 == compare(v1, v2) && 0 != compare(v0, v3))
                {
                    out.push_back(v3);
                    flags[i] = false;
                    updated = true;
                    break;
                }
                else if (0 == compare(v1, v3) && 0 != compare(v0, v2))
                {
                    out.push_back(v2);
                    flags[i] = false;
                    updated = true;
                    break;
                }
                else if (0 == compare(v0, v2) && 0 != compare(v1, v3))
                {
                    out.insert(out.begin(), v3);
                    flags[i] = false;
                    updated = true;
                    break;
                }
                else if (0 == compare(v0, v3) && 0 != compare(v1, v2))
                {
                    out.insert(out.begin(), v2);
                    flags[i] = false;
                    updated = true;
                    break;
                }
                else if (0 == compare(v0, v2) && 0 == compare(v1, v3))
                    return 2;
                else if (0 == compare(v0, v3) && 0 == compare(v1, v2))
                    return 2;
            }
            if (!updated)
                break;
        }
        return 3;
    }
    vertices get_surroundings(const vertex& c0, const vertex& c,
        double w, double h)
    {
        vertices res;
        double dx = c0.x - c.x, dy = c0.y - c.y;
        if (dx != 0)
        {
            vertex L = { 0, (c0.y - c.y) / (c0.x - c.x) * (-c.x) + c.y };
            vertex R = { w, (c0.y - c.y) / (c0.x - c.x) * (w - c.x) + c.y };
            res.push_back(L);
            res.push_back(R);
        }
        if (dy != 0)
        {
            vertex B = { (c0.x - c.x) / (c0.y - c.y) * (-c.y) + c.x, 0 };
            vertex T = { (c0.x - c.x) / (c0.y - c.y) * (h - c.y) + c.x, h };
            res.push_back(B);
            res.push_back(T);
        }
        return res;
    }
    segment validate_segment(const segment& src, double w, double h)
    {
        double prec = 1e-7;
        segment res = src;
        auto points = get_surroundings(src.first, src.second, w, h);
        auto c0 = src.first, c1 = src.second;
        if ((c0.x < 0 || c0.x > w || c0.y < 0 || c0.y > h) &&
            !(c1.x < 0 || c1.x > w || c1.y < 0 || c1.y > h))
        {
            auto tmp = c0;
            c0 = c1;
            c1 = tmp;
            goto VALIDATE_CALCULATE;
        }
        else if (!(c0.x < 0 || c0.x > w || c0.y < 0 || c0.y > h) &&
            (c1.x < 0 || c1.x > w || c1.y < 0 || c1.y > h))
        {
VALIDATE_CALCULATE:
            vertex v0 = { c1.x - c0.x, c1.y - c0.y };
            double len0 = sqrt(v0.x * v0.x + v0.y * v0.y);
            v0.x /= len0;
            v0.y /= len0;
            len0 = 1;
            for (auto it = points.begin(); it != points.end();)
            {
                vertex v1 = { (*it).x - c0.x, (*it).y - c0.y };
                double len1 = sqrt(v1.x * v1.x + v1.y * v1.y);
                v1.x /= len1;
                v1.y /= len1;
                len1 = 1;
                double dot = v0.x * v1.x + v0.y * v1.y;
                double cosT = dot / len0 / len1;
                if (abs(cosT - 1) < prec)
                    it++;
                else
                    it = points.erase(it);
            }
            if (points.size() > 0)
            {
                size_t pos = 0;
                double d = distance(points[pos], c0);
                for (size_t j = 1; j < points.size(); j++)
                {
                    double _d = distance(points[j], c0);
                    if (_d < d)
                    {
                        pos = j;
                        d = _d;
                    }
                }
                res = { c0, points[pos] };
            }
        }
        return res;
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
