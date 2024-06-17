#include "poisson.h"

#include <cstdlib>
#include <cmath>
#include <algorithm>

#include "randomize.h"

namespace vl
{
    static const size_t poisson_dimension = 2;
    vertices poisson_distribute(double w, double h, double r, size_t k)
    {
        vertices res;
        struct point
        {
            vertex v;
            size_t location = (size_t)-1;
        };
        std::vector<point> points;
        double r1 = r, r2 = 2.0 * r;
        double cell_size = r / sqrt(double(poisson_dimension));
        size_t rows = (size_t)ceil(h / cell_size);
        size_t cols = (size_t)ceil(w / cell_size);
        std::vector<std::vector<size_t>> grids(rows,
            std::vector<size_t>(cols, (size_t)-1));
        point p0;
        p0.v.x = w * random_real();
        p0.v.y = h * random_real();
        size_t row = (size_t)floor(p0.v.y / cell_size);
        size_t col = (size_t)floor(p0.v.x / cell_size);
        size_t p0_idx = points.size();
        p0.location = p0_idx;
        points.push_back(p0);
        grids[row][col] = p0_idx;
        std::vector<size_t> activities;
        activities.push_back(p0_idx);
        auto check_distance = [&r, &rows, &cols, &grids, &points](point pt,
            size_t Row, size_t Col, int dx, int dy)->bool {
            if (Row + dx < 0 || Col + dy < 0 ||
                Row + dx >= rows || Col + dy >= cols)
                return false;
            size_t idx = grids[Row + dx][Col + dy];
            if (idx == (size_t)-1)
                return false;
            point p = points[idx];
            double distance = sqrt(pow(pt.v.x - p.v.x, 2) +
                pow(pt.v.y - p.v.y, 2));
            return distance < r;
        };
        static const std::vector<std::pair<int, int>> offsets = {
                        { -1, -2 }, { 0, -2 }, { 1, -2 },
            { -2, -1 }, { -1, -1 }, { 0, -1 }, { 1, -1 }, { 2, -1 },
            { -2, 0 },  { -1, 0 },/*{0, 0},  */{ 1, 0 },  { 2, 0 },
            { -2, 1 },  { -1, 1 },  { 0, 1 },  { 1, 1 },  { 2, 1 },
                        { -1, 2 },  { 0, 2 },  { 1, 2 },
        };
        while (!activities.empty())
        {
            size_t r_idx = random_uint() % activities.size();
            size_t pi_idx = activities[r_idx];
            point pi = points[pi_idx];
            bool found = false;
            for (size_t i = 0; i < k; i++)
            {
                double A = 1.0 / (r2 * r2 - r1 * r1);
                double R = sqrt(random_real() / A + r1 * r1);
                double T = random_real() * 2.0 * atan(1.0) * 4.0;
                double cx = R * cos(T), cy = R * sin(T);
                point pk = { pi.v.x + cx, pi.v.y + cy };
                if (pk.v.x < 0 || pk.v.y < 0 || pk.v.x > w || pk.v.y > h)
                    continue;
                row = (size_t)float(pk.v.y / cell_size);
                col = (size_t)floor(pk.v.x / cell_size);
                if (grids[row][col] != (size_t)-1)
                    continue;
                bool invalid = false;
                for (auto& D : offsets)
                    invalid = invalid || check_distance(pk, row, col,
                        D.first, D.second);
                if (!invalid)
                {
                    size_t pk_idx = points.size();
                    pk.location = pk_idx;
                    points.push_back(pk);
                    grids[row][col] = pk_idx;
                    activities.push_back(pk_idx);
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                auto it = std::find(activities.begin(), activities.end(),
                    pi_idx);
                if (it != activities.end())
                    activities.erase(it);
            }
        }
        for (auto& p : points)
            res.push_back(p.v);
        return res;
    }
} // namespace vl
