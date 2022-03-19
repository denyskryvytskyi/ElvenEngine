#pragma once

#include <cmath>
#include <ostream>

namespace lia
{
    struct vec3
    {
        float x, y, z;

        vec3() = default;

        vec3(float xx, float yy, float zz)
            : x(xx), y(yy), z(zz)
        {}

        vec3(float scalar)
            : x(scalar), y(scalar), z(scalar)
        {}

        vec3& operator*=(float scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;

            return (*this);
        }

        vec3& operator/=(float scalar)
        {
            scalar = 1.0f / scalar;
            x *= scalar;
            y *= scalar;
            z *= scalar;

            return (*this);
        }

        vec3& operator+=(const vec3& v)
        {
            x += v.x;
            y += v.y;
            z += v.z;

            return (*this);
        }

        vec3& operator-=(const vec3& v)
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;

            return (*this);
        }
    };

    inline vec3 operator*(const vec3& v, float scalar)
    {
        return vec3(v.x * scalar, v.y * scalar, v.z * scalar);
    }

    inline vec3 operator/(const vec3& v, float scalar)
    {
        scalar = 1.0f / scalar;
        return vec3(v.x * scalar, v.y * scalar, v.z * scalar);
    }

    inline vec3 operator-(const vec3& v)
    {
        return vec3(-v.x, -v.y, -v.z);
    }

    inline float magnitude(const vec3& v)
    {
        return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    }

    inline vec3 normalize(const vec3& v)
    {
        return v / magnitude(v);
    }

    inline vec3 operator+(const vec3& first, const vec3& second)
    {
        return (vec3(first.x + second.x, first.y + second.y, first.z + second.z));
    }

    inline vec3 operator-(const vec3& first, const vec3& second)
    {
        return (vec3(first.x - second.x, first.y - second.y, first.z - second.z));
    }

    inline std::ostream& operator<<(std::ostream& stream, const vec3& vector)
    {
        stream << "vec3(" << vector.x << ", " << vector.y << ", " << vector.z << ")";

        return stream;
    }

    inline float dot(const vec3& v1, const vec3& v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    inline vec3 cross(const vec3& v1, const vec3& v2)
    {
        return vec3(v1.y * v2.z - v1.z * v2.y,
            v1.z * v2.x - v1.x * v2.z,
            v1.x * v2.y - v1.y * v2.x);
    }

    inline vec3 project(const vec3& v1, const vec3& v2)
    {
        return v1 * (dot(v1, v2) / dot(v2, v2));
    }
    
    inline vec3 reject(const vec3& v1, const vec3& v2)
    {
        return v1 - v2 * (dot(v1, v2) / dot(v2, v2));
    }
}