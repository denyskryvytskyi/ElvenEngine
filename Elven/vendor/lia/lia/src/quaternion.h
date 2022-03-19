#pragma once
#include "vec3.h"
#include "mat4.h"

namespace lia
{
    struct quaternion
    {
        float x, y, z, w;

        quaternion()
            : x(0), y(0), z(0), w(1)
        {}

        quaternion(float xx, float yy, float zz, float s)
            : x(xx), y(yy), z(zz), w(s)
        {}

        quaternion(const vec3& v, float s)
            : x(v.x), y(v.y), z(v.z), w(s)
        {}

        vec3& GetVectorPart()
        {
            return reinterpret_cast<vec3&>(x);
        }

        const vec3& GetVectorPart() const
        {
            return reinterpret_cast<const vec3&>(x);
        }

        quaternion& operator+=(const quaternion& q)
        {
            x += q.x;
            y += q.y;
            z += q.z;
            w += q.w;
            return *this;
        }

        quaternion& operator-=(const quaternion& q)
        {
            x -= q.x;
            y -= q.y;
            z -= q.z;
            w -= q.w;
            return *this;
        }

        quaternion& operator*=(const quaternion& q)
        {
            x *= q.x;
            y *= q.y;
            z *= q.z;
            w *= q.w;
            return *this;
        }

        quaternion& operator*=(float scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            w *= scalar;
            return *this;
        }

        quaternion& operator/=(float scalar)
        {
            x /= scalar;
            y /= scalar;
            z /= scalar;
            w /= scalar;
            return *this;
        }

        float& operator[](int index)
        {
            return ((&x)[index]);
        }

        const float& operator[](int index) const
        {
            return ((&x)[index]);
        }

        /**
        * Convert matrix to quaternion
        */
        void setRotationMatrix(const mat4& m)
        {
            float m00 = m(0, 0);
            float m11 = m(1, 1);
            float m22 = m(2, 2);
            float sum = m00 + m11 + m22;

            if (sum > 0.0F)
            {
                w = sqrt(sum + 1.0f) * 0.5f;
                float f = 0.25F / w;

                x = (m(2, 1) - m(1, 2)) * f;
                y = (m(0, 2) - m(2, 0)) * f;
                z = (m(1, 0) - m(0, 1)) * f;
            }
            else if ((m00 > m11) && (m00 > m22))
            {
                x = sqrt(m00 - m11 - m22 + 1.0f) * 0.5f;
                float f = 0.25f / x;

                y = (m(1, 0) + m(0, 1)) * f;
                z = (m(0, 2) + m(2, 0)) * f;
                w = (m(2, 1) - m(1, 2)) * f;
            }
            else if (m11 > m22)
            {
                y = sqrt(m11 - m00 - m22 + 1.0f) * 0.5f;
                float f = 0.25f / y;

                x = (m(1, 0) + m(0, 1)) * f;
                z = (m(2, 1) + m(1, 2)) * f;
                w = (m(0, 2) - m(2, 0)) * f;
            }
            else
            {
                w = sqrt(m22 - m00 - m11 + 1.0f) * 0.5f;
                float f = 0.25f / w;

                x = (m(0, 2) + m(2, 0)) * f;
                y = (m(2, 1) + m(1, 2)) * f;
                z = (m(1, 0) - m(0, 1)) * f;
            }
        }

        /**
        * Convert quaternion to matrix
        */
        mat4 getRotationMatrix()
        {
            float x2 = x * x;
            float y2 = y * y;
            float z2 = z * z;
            float xy = x * y;
            float xz = x * z;
            float yz = y * z;
            float wx = w * x;
            float wy = w * y;
            float wz = w * z;

            return mat4(1.0F - 2.0F * (y2 + z2), 2.0F * (xy - wz), 2.0F * (xz + wy), 0,
                        2.0F * (xy + wz), 1.0F - 2.0F * (x2 + z2), 2.0F * (yz - wx), 0,
                        2.0F * (xz - wy), 2.0F * (yz + wx), 1.0F - 2.0F * (x2 + y2), 0,
                        0, 0, 0, 1);
        }
    };

    inline const quaternion operator+(const quaternion& q1, const quaternion& q2)
    {
        return quaternion(q1.x + q2.x, q1.y + q2.y, q1.z + q2.z, q1.w + q2.w);
    }

    inline const quaternion operator-(const quaternion& q1, const quaternion& q2)
    {
        return quaternion(q1.x - q2.x, q1.y - q2.y, q1.z - q2.z, q1.w - q2.w);
    }

    inline quaternion& operator*(const quaternion& q1, const quaternion& q2)
    {
        return quaternion(q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
            q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x,
            q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w,
            q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z);
    }

    inline const quaternion operator*(const quaternion& q, float scalar)
    {
        return quaternion(q.x * scalar, q.y * scalar, q.z * scalar, q.w * scalar);
    }

    inline const quaternion operator/(const quaternion& q, float scalar)
    {
        return quaternion(q.x / scalar, q.y / scalar, q.z / scalar, q.w / scalar);
    }

    inline quaternion Conjugate(const quaternion& q)
    {
        return quaternion(-q.x, -q.y, -q.z, q.w);
    }

    inline vec3& rotate(const vec3& v, const quaternion& q)
    {
        const vec3& b = q.GetVectorPart();
        float b2 = b.x * b.x + b.y * b.y + b.z * b.z;
        return (v * (q.w * q.w - b2) + b * (dot(v, b) * 2.0f)
            + cross(b, v) * (q.w * 2.0f));
    }

    /**
    * @param angle Angle in radians
    */
    inline quaternion rotation(float angle, const vec3& unitVec)
    {
        float halfAngle = angle * 0.5f;
        return quaternion((unitVec * sin(halfAngle)), cos(halfAngle));
    }

    /**
    * @param angle Angle in radians
    */
    inline quaternion rotationX(float angle)
    {
        float halfAngle = angle * 0.5f;
        return quaternion(sin(halfAngle), 0, 0, cos(halfAngle));
    }
    
    /**
    * @param angle Angle in radians
    */
    inline quaternion rotationY(float angle)
    {
        float halfAngle = angle * 0.5f;
        return quaternion(0, sin(halfAngle), 0, cos(halfAngle));
    }
    
    /**
    * @param angle Angle in radians
    */
    inline quaternion rotationZ(float angle)
    {
        float halfAngle = angle * 0.5f;
        return quaternion(0, 0, sin(halfAngle), cos(halfAngle));
    }
}