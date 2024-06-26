
#if !defined(_VORONOI_LINES_VORONOI_H_)
#define _VORONOI_LINES_VORONOI_H_

#include "geometry.h"

namespace vl
{
    using unique_indices = std::set<size_t>;
    using uniques = std::vector<unique_indices>;
    class voronoi final
    {
        friend class map;
    public:
        voronoi(const vertices& _points, const triangles& _facets,
            double w, double h);
        const vertices& get_points() const;
        const triangles& get_facets() const;
        const edges& get_borders() const;
        unique_indices get_borders_by_point(size_t id) const;
    private:
        voronoi();
        size_t push_border_unique(const edge& e);
    private:
        double width, height;
        vertices points;
        triangles facets;
        edges borders;
        uniques p2f, p2b;
        uniques b2f, f2b;
        uniques p2v;
    };

    triangles triangulate(const vertices& src, double w, double h);
} // namespace vl

#endif
