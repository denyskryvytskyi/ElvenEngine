#pragma once

#include <cmath>
#include <ostream>

namespace lia
{
    struct vec2
    {
        float x, y;

        vec2() = default;

        vec2(float xx, float yy)
            : x(xx), y(yy)
        {}

        vec2(float scalar)
            : x(scalar), y(scalar)
        {}

        vec2& operator*=(float scalar)
        {
            x *= scalar;
            y *= scalar;

            return (*this);
        }

        vec2& operator/=(float scalar)
        {
            scalar = 1.0f / scalar;
            x *= scalar;
            y *= scalar;

            return (*this);
        }

        vec2& operator+=(const vec2& v)
        {
            x += v.x;
            y += v.y;

            return (*this);
        }

        vec2& operator-=(const vec2& v)
        {
            x -= v.x;
            y -= v.y;

            return (*this);
        }
    };

    inline vec2 operator*(const vec2& v, float scalar)
    {
        return vec2(v.x * scalar, v.y * scalar);
    }

    inline vec2 operator/(const vec2& v, float scalar)
    {
        scalar = 1.0f / scalar;
        return vec2(v.x * scalar, v.y * scalar);
    }

    inline vec2 operator-(const vec2& v)
    {
        return vec2(-v.x, -v.y);
    }

    inline float magnitude(const vec2& v)
    {
        return sqrt(v.x * v.x + v.y * v.y);
    }

    inline vec2 normalize(const vec2& v)
    {
        return v / magnitude(v);
    }

    inline vec2 operator+(const vec2& first, const vec2& second)
    {
        return (vec2(first.x + second.x, first.y + second.y));
    }

    inline vec2 operator-(const vec2& first, const vec2& second)
    {
        return (vec2(first.x - second.x, first.y - second.y));
    }

    inline std::ostream& operator<<(std::ostream& stream, const vec2& vector)
    {
        stream << "vec3(" << vector.x << ", " << vector.y << ")";

        return stream;
    }

    inline vec2 clamp(const vec2& vec, const vec2& min, const vec2& max)
    {
        vec2 clamped = vec;

        // Clamp the x value.
        if (clamped.x < min.x)
            clamped.x = min.x;
        if (clamped.x > max.x)
            clamped.x = max.x;

        // Clamp the y value.
        if (clamped.y < min.y)
            clamped.y = min.y;
        if (clamped.y > max.y)
            clamped.y = max.y;

        return clamped;
    }

    inline float dot(const vec2& v1, const vec2& v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    inline lia::vec2 rotatePoint(float angle, lia::vec2 point, lia::vec2 origin)
    {
        return vec2(cos(angle) * (point.x - origin.x) - sin(angle) * (point.y - origin.y) + origin.x,
                    sin(angle) * (point.x - origin.x) + cos(angle) * (point.y - origin.y) + origin.y);
    }
}