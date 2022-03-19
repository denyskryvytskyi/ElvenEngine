#pragma once

#include <cmath>
#include <ostream>

namespace lia
{
    struct vec4
    {
        float x, y, z, w;

        vec4() = default;

        vec4(float xx, float yy, float zz, float ww)
            : x(xx), y(yy), z(zz), w(ww)
        {}

        vec4(float scalar)
            : x(scalar), y(scalar), z(scalar), w(scalar)
        {}

        float& operator [](int i)
        {
            return ((&x)[i]);
        }

        const float& operator [](int i) const
        {
            return ((&x)[i]);
        }

        vec4& operator*=(float scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            w *= scalar;

            return (*this);
        }

        vec4& operator/=(float scalar)
        {
            scalar = 1.0f / scalar;
            x *= scalar;
            y *= scalar;
            z *= scalar;
            w *= scalar;

            return (*this);
        }

        vec4& operator+=(const vec4& v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
            w += v.z;

            return (*this);
        }

        vec4& operator-=(const vec4& v)
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            w -= v.z;

            return (*this);
        }
    };

    inline vec4 operator*(const vec4& v, float scalar)
    {
        return vec4(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
    }

    inline vec4 operator/(const vec4& v, float scalar)
    {
        scalar = 1.0f / scalar;
        return vec4(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
    }

    inline vec4 operator-(const vec4& v)
    {
        return vec4(-v.x, -v.y, -v.z, -v.w);
    }

    inline std::ostream& operator<<(std::ostream& stream, const vec4& vector)
    {
        stream << "vec3(" << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << ")";

        return stream;
    }

    inline float dot(const vec4& v1, const vec4& v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
    }
}