
#if !defined(_VORONOI_LINES_GAME_H_)
#define _VORONOI_LINES_GAME_H_

#include <cstdint>

#include "voronoi.h"

namespace vl
{
    class map final
    {
    public:
        map(const voronoi& v);
        const voronoi& get_distribution() const;
        const std::vector<int32_t>& get_weights() const;
    private:
        void distribute();
    private:
        voronoi distribution;
        std::vector<int32_t> weights;
    };
} // namespace vl

#endif
