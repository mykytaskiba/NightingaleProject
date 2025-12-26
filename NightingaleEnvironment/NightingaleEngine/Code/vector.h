#pragma once
//Mathmatical vector
#include "defines.h"

template < typename T, uint DIM >
struct Vector {

private:
    T data[DIM];

public:

    static Vector<T, DIM> garbage_vector() {
        return Vector<T, DIM>();
    }

    Vector(Vector const& other) = default;

    Vector() {}

    Vector(T x, T y) requires (DIM == 2) {
        data[0] = x;
        data[1] = y;
    }

    Vector(T x, T y, T z) requires (DIM == 3) {
        data[0] = x;
        data[1] = y;
        data[2] = z;
    }

    Vector(T x, T y, T z, T w) requires (DIM == 4) {
        data[0] = x;
        data[1] = y;
        data[2] = z;
        data[3] = w;
    }


    T& operator[](uint index) {
        assert(index < DIM);
        return data[index];
    }

    T operator[](uint index) const {
        assert(index < DIM);
        return data[index];
    }

    // = equal operator

    Vector<T, DIM>& operator+=(Vector<T, DIM> const& other) {
        for (uint loop = 0; loop < DIM; loop++) {
            data[loop] += other.data[loop];
        }
        return *this;
    }

    Vector<T, DIM>& operator-=(Vector<T, DIM> const& other) {
        for (uint loop = 0; loop < DIM; loop++) {
            data[loop] -= other.data[loop];
        }
        return *this;
    }

    Vector<T, DIM>& operator*=(float scalar) {
        for (uint loop = 0; loop < DIM; loop++) {
            data[loop] *= scalar;
        }
        return *this;
    }

    Vector<T, DIM>& operator/=(float scalar) {
        for (uint loop = 0; loop < DIM; loop++) {
            data[loop] /= scalar;
        }
        return *this;
    }

    float dot(Vector<T, DIM> const& other) const {
        float dotProduct = 0.0f;
        for (uint loop = 0; loop < DIM; loop++) {
            dotProduct += data[loop] * other.data[loop];
        }
        return dotProduct;
    }

    Vector<T, DIM + 1u> add_dimension(T value) const {
        Vector<T, DIM + 1u> result = Vector<T,DIM+1u>::garbage_vector();

        for (uint loop = 0; loop < DIM; loop++) {
            result[loop] = data[loop];
        }

        result[DIM] = value;
        return result;
    }

    Vector<T, DIM - 1u> truncate_dimension() const {
        Vector<T, DIM - 1u> result = Vector<T, DIM - 1u>::garbage_vector();

        for (uint loop = 0; loop < DIM - 1u; loop++) {
            result[loop] = data[loop];
        }

        return result;
    }


    float magnitude_squared() {
        float magn = 0.0f;
        for (uint loop = 0; loop < DIM; loop++) {
            magn += data[loop] * data[loop];
        }
        return magn;
    }
    float magnitude() {
        return sqrt(magnitude_squared());
    }

    Vector<T,DIM>& normalize_equal() {
        float magn = magnitude();
        if (magn <= 0.0f) {
            return *this;
        }
        for (uint loop = 0; loop < DIM; loop++) {
            data[loop] /= magn;
        }
        return *this;
    }

    Vector<T,DIM> normalized() const {
        Vector<T, DIM> result(*this);
        result.normalize_equal();
        return result;
    }

    //Copy creating operators
    
    Vector<T, DIM> operator+(Vector<T, DIM> const& other) const {
        Vector<T, DIM> result(*this);
        result += other;
        return result;
    }

    Vector<T, DIM> operator-(Vector<T, DIM> const& other) const {
        Vector<T, DIM> result(*this);
        result -= other;
        return result;
    }

    Vector<T, DIM> operator*(float scalar) const {
        Vector<T, DIM> result(*this);
        result *= scalar;
        return result;
    }

    Vector<T, DIM> operator/(float scalar) const {
        Vector<T, DIM> result(*this);
        result /= scalar;
        return result;
    }

    Vector<T, DIM> cross(Vector<T, DIM> const& other) requires (DIM == 3) {
        Vector<T, DIM> result;
        result.data[0] = data[1] * other.data[2] - data[2] * other.data[1];
        result.data[1] = data[2] * other.data[0] - data[0] * other.data[2];
        result.data[2] = data[0] * other.data[1] - data[1] * other.data[0];
        return result;
    }

    static Vector<T, DIM> interpolate(Vector<T, DIM> const& a, Vector<T, DIM> const& b, float t) {
        Vector<T, DIM> result;
        float oneMinusT = 1.0f - t;

        for (uint loop = 0; loop < DIM; ++loop) {
            result[loop] = oneMinusT * a.data[loop] + t * b.data[loop];
        }
        return result;
    }

};

template < typename T, uint DIM >
static Vector<T, DIM> operator*(float scalar, Vector<T, DIM>const& vector) {
    return (vector * scalar);
}

template < typename T, uint DIM >
static Vector<T, DIM> operator/(float scalar, Vector<T, DIM>const& vector) {
    return (vector / scalar);
}







//Vector defines
using Vector2 = Vector<float, 2>;
using Vector3 = Vector<float, 3>;
using Vector4 = Vector<float, 4>;

using Vector2i = Vector<int, 3>;
using Vector3i = Vector<int, 3>;
using Vector4i = Vector<int, 3>;
