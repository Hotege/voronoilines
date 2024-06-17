#include "randomize.h"

#include <cstdlib>
#include <cstdint>

namespace vl
{
    void randomize(unsigned int seed)
    {
        srand(seed);
    }
    size_t random_uint()
    {
        union
        {
            unsigned char c[sizeof(size_t)];
            size_t s;
        } x;
        for (int i = 0; i < sizeof(size_t); i++)
            x.c[i] = rand() % 256;
        return x.s;
    }
    double random_real()
    {
        auto RandomUInt64 = []()->uint64_t {
            union
            {
                unsigned char c[8];
                uint64_t u;
            } x;
            for (int i = 0; i < 8; i++)
                x.c[i] = rand() % 256;
            return x.u;
        };
        union
        {
            double d;
            uint64_t u;
        } x;
        x.u = (RandomUInt64() >> 12) | 0x3FF0000000000000ULL;
        return x.d - 1.0;
    }
} // namespace vl
