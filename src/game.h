
#if !defined(_VORONOI_LINES_GAME_H_)
#define _VORONOI_LINES_GAME_H_

#include <cstdint>

#include "voronoi.h"

namespace vl
{
    class map final
    {
        friend class game;
    public:
        map(const voronoi& v);
        const voronoi& get_distribution() const;
        const std::vector<int32_t>& get_weights() const;
    private:
        map();
        void distribute();
    private:
        voronoi distribution;
        std::vector<int32_t> weights;
    };
    class game final
    {
    public:
        struct init_data
        {
            double width;
            double height;
            double radius;
            size_t k = 30;
            size_t num_players = 2;
        };
    public:
        game(const init_data& init);
        game(unsigned int s, const init_data& init);
        const map& get_data() const;
        const std::vector<bool>& get_borders_flags() const;
        size_t get_valid_verticals_count() const;
        size_t get_turn() const;
        const std::vector<size_t>& get_occupation() const;
        int play(size_t id);
        std::vector<size_t> get_scores() const;
        bool is_subsistent() const;
    private:
        void initialize(const init_data& init);
    private:
        unsigned int seed;
        init_data profiles;
        map data;
        size_t turn = 0;
        size_t current = 0;
        std::vector<bool> borders_flags;
        std::vector<size_t> occupation;
    };
} // namespace vl

#endif
