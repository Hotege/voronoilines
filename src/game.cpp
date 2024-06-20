
#include "game.h"
#include "randomize.h"

namespace vl
{
    map::map(const voronoi& v) : distribution(v)
    {
        weights.resize(v.get_points().size(), 1);
        distribute();
    }
    const voronoi& map::get_distribution() const
    {
        return distribution;
    }
    const std::vector<int32_t>& map::get_weights() const
    {
        return weights;
    }
    void map::distribute()
    {
        for (size_t i = 0; i < distribution.get_points().size(); i++)
        {
            double r = random_real();
            if (r < 140.0 / 900)
                weights[i] = 1;
            else if (r >= 140.0 / 900 && r < 277.223 / 900)
                weights[i] = 2;
            else if (r >= 227.223 / 900 && r < 407.753 / 900)
                weights[i] = 3;
            else if (r >= 407.753 / 900 && r < 527.063/ 900)
                weights[i] = 4;
            else if (r >= 527.063 / 900 && r < 627.063 / 900)
                weights[i] = 5;
            else if (r >= 627.063 / 900 && r < 707.753 / 900)
                weights[i] = 6;
            else if (r >= 707.753 / 900 && r < 777.223 / 900)
                weights[i] = 7;
            else if (r >= 777.223 / 900 && r < 840.0 / 900)
                weights[i] = 8;
            else
                weights[i] = 9;
        }
    }
} // namespace vl
