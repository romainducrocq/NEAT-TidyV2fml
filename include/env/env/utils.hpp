#ifndef _ENV_UTILS_HPP
#define _ENV_UTILS_HPP

#include <iostream>

#include <cmath>
#include <algorithm>

#include "env/conf.hpp"

namespace CONST
{
    const float WIDTH = static_cast<float>(CONF::WIN_W);
    const float HEIGHT = static_cast<float>(CONF::WIN_H);
    const float FLOOR_Y = 50.f;
}

namespace math
{
    const float PI = 3.14159265358979323846f;

    inline float rad_to_deg(float deg)
    {
        return deg * 180.f / math::PI;
    }

    struct Vector2f
    {
        float x, y;

        Vector2f()
            : x(0.f), y(0.f)
        {}

        Vector2f(float x, float y)
            : x(x), y(y)
        {}

        bool operator<(const Vector2f& other) const
        {
            return (x < other.x) || ((!(other.x < x)) && (y < other.y));
        }
    };

    inline float clip(float min_clip, float max_clip, float x)
    {
        return std::max(min_clip, std::min(max_clip, x));
    }

    inline float dist(const math::Vector2f &p1, const math::Vector2f &p2)
    {
        return std::sqrt(std::pow(p2.x - p1.x, 2.f) + std::pow(p2.y - p1.y, 2.f));
    }

    inline float linterp(float x, float x1, float x2, float y1, float y2)
    {
        return y1 + ((std::min(std::max(x, x1), x2) - x1) / (x2 - x1)) * (y2 - y1);
    }

    inline void point_on_circle(float theta, float radius, float x_orig, float y_orig, math::Vector2f& p)
    {
        p.x = x_orig + std::cos(theta) * radius;
        p.y = y_orig + std::sin(theta) * radius;
    }
}

#endif
