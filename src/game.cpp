
#include "game.h"
#include <cstdio>
#include <ctime>
#include "randomize.h"
#include "poisson.h"

namespace vl
{
    map::map()
    {
    }
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
    game::game(const init_data& init) : seed((unsigned int)time(nullptr))
    {
        initialize(init);
    }
    game::game(unsigned int s, const init_data& init) : seed(s)
    {
        initialize(init);
    }
    const map& game::get_data() const
    {
        return data;
    }
    const std::vector<bool>& game::get_borders_flags() const
    {
        return borders_flags;
    }
    size_t game::get_valid_verticals_count() const
    {
        size_t cnt = 0;
        for (auto& b : data.get_distribution().get_borders())
            if (b.get_vertical_valid())
                cnt++;
        return cnt;
    }
    size_t game::get_turn() const
    {
        return turn;
    }
    const std::vector<size_t>& game::get_occupation() const
    {
        return occupation;
    }
    int game::play(size_t id)
    {
        int res = 0;
        auto& V = data.get_distribution();
        auto& borders = V.get_borders();
        auto& b = borders[id];
        if (!b.get_vertical_valid() || borders_flags[id])
            return res;
        borders_flags[id] = true;
        res |= 1;
        auto i0 = b.from(), i1 = b.to();
        auto bs0 = V.get_borders_by_point(i0);
        auto bs1 = V.get_borders_by_point(i1);
        bool f0 = true, f1 = true;
        for (auto& bi : bs0)
            if (borders[bi].get_vertical_valid() && !borders_flags[bi])
            {
                f0 = false;
                break;
            }
        for (auto& bi : bs1)
            if (borders[bi].get_vertical_valid() && !borders_flags[bi])
            {
                f1 = false;
                break;
            }
        if (f0 || f1)
        {
            if (f0)
                occupation[i0] = current;
            if (f1)
                occupation[i1] = current;
            res |= 2;
        }
        else
        {
            current++;
            if (current >= profiles.num_players)
                current = 0;
        }
        turn++;
        return res;
    }
    std::vector<size_t> game::get_scores() const
    {
        std::vector<size_t> res(profiles.num_players, 0);
        for (size_t i = 0; i < occupation.size(); i++)
            if (occupation[i] < profiles.num_players)
                res[occupation[i]] += data.get_weights()[i];
        return res;
    }
    bool game::is_subsistent() const
    {
        auto& V = data.get_distribution();
        auto& borders = V.get_borders();
        for (size_t i = 0; i < borders_flags.size(); i++)
            if (borders[i].get_vertical_valid() && !borders_flags[i])
                return true;
        return false;
    }
    void game::initialize(const init_data& init)
    {
        randomize(seed);
        profiles = init;
        auto& W = init.width;
        auto& H = init.height;
        auto& R = init.radius;
        auto& K = init.k;
        auto vertices = poisson_distribute(W, H, R, K);
        auto triangles = triangulate(vertices, W, H);
        data = map(voronoi(vertices, triangles, W, H));
        auto& distribution = data.get_distribution();
        borders_flags.resize(distribution.get_borders().size(), false);
        occupation.resize(distribution.get_points().size(), init.num_players);
    }
} // namespace vl
