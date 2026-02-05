#pragma once
#include "defines.h"
#include "vector.h"
#include <concepts>
#include <cmath>

template <typename T, unsigned int DIM_ROW, unsigned int DIM_COL>
class Matrix {

private:

    using COL_VECTOR = Vector<T, DIM_ROW>;
    COL_VECTOR data[DIM_COL];


public:

    Matrix() = default;
    Matrix(Matrix<T, DIM_ROW, DIM_COL> const& other) = default;

    COL_VECTOR& operator[](uint index_col) {
        assert(index_col < DIM_COL);
        return data[index_col];
    }

    COL_VECTOR operator[](uint index_col) const {
        assert(index_col < DIM_COL);
        return data[index_col];
    }

    Matrix& transpose_equal() {
        for (uint loop_col = 0; loop_col < DIM_COL; ++loop_col) {
            for (uint loop_row = loop_col+1; loop_row < DIM_ROW; ++loop_row) {
                std::swap(data[loop_row][loop_col],data[loop_col][loop_row]);
            }
        }
        return *this;
    }

    Matrix transpose() const {
        Matrix result{ *this };
        return result.transpose_equal();
    }

    Vector<T, DIM_ROW> operator*(const Vector<T, DIM_COL>& vec) const {
        Vector<T, DIM_ROW> result;

        for (uint row = 0; row < DIM_ROW; ++row) {
            result[row] = 0;
            for (uint col = 0; col < DIM_COL; ++col) {
                result[row] += data[col][row] * vec[col];
            }
        }

        return result;
    }

    template <unsigned int OUTER_DIM>
    Matrix<T, DIM_ROW, OUTER_DIM> operator*(const Matrix<T, DIM_COL, OUTER_DIM>& rhs) const {
        Matrix<T, DIM_ROW, OUTER_DIM> result;
        
        for (uint col = 0; col < OUTER_DIM; ++col) {
            for (uint row = 0; row < DIM_ROW; ++row) {
                result.data[col][row] = 0;
                for (uint k = 0; k < DIM_COL; ++k) {
                    result.data[col][row] +=
                        data[k][row] * rhs.data[col][k];
                }
            }
        }

        return result;
    }

    Matrix& make_identity()
        requires (std::same_as<T, float>&& DIM_ROW == 4 && DIM_COL == 4) {
        Matrix& m = *this;

        //transposed for memory locality
        m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
        m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = 0;
        m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = 0;
        m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;

        return *this;
    }

    Matrix& make_translation(T x, T y, T z)
        requires (std::same_as<T, float>&& DIM_ROW == 4 && DIM_COL == 4) {

        Matrix& m = *this;

        //transposed for memory locality
        m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0; 
        m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = 0;
        m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = 0;
        m[3][0] = x; m[3][1] = y; m[3][2] = z; m[3][3] = 1;
        
        return *this;
    }

    Matrix& make_translation(Vector3 const& vec)
        requires (std::same_as<T, float>&& DIM_ROW == 4 && DIM_COL == 4) {
        return make_translation(vec[0], vec[1], vec[2]);
    }

    Matrix& make_rotation_x(T angle)
        requires (std::same_as<T, float>&& DIM_ROW == 4 && DIM_COL == 4) {

        Matrix& m = *this;
        T c = cos(angle);
        T s = sin(angle);

        //transposed for memory locality
        m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
        m[1][0] = 0; m[1][1] = c; m[1][2] = s; m[1][3] = 0;
        m[2][0] = 0; m[2][1] =-s; m[2][2] = c; m[2][3] = 0;
        m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;

        return *this;
    }
    Matrix& make_rotation_y(T angle)
        requires (std::same_as<T, float>&& DIM_ROW == 4 && DIM_COL == 4) {

        Matrix& m = *this;
        T c = cos(angle);
        T s = sin(angle);

        //transposed for memory locality
        m[0][0] = c; m[0][1] = 0; m[0][2] =-s; m[0][3] = 0;
        m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = 0;
        m[2][0] = s; m[2][1] = 0; m[2][2] = c; m[2][3] = 0;
        m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;

        return *this;
    }
    Matrix& make_rotation_z(T angle)
        requires (std::same_as<T, float>&& DIM_ROW == 4 && DIM_COL == 4) {

        Matrix& m = *this;
        T c = cos(angle);
        T s = sin(angle);

        //transposed for memory locality
        m[0][0] = c; m[0][1] = s; m[0][2] = 0; m[0][3] = 0;
        m[1][0] =-s; m[1][1] = c; m[1][2] = 0; m[1][3] = 0;
        m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = 0;
        m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;

        return *this;
    }
    Matrix& make_scale(T x, T y, T z)
        requires (std::same_as<T, float>&& DIM_ROW == 4 && DIM_COL == 4) {

        Matrix& m = *this;

        //transposed for memory locality
        m[0][0] = x; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
        m[1][0] = 0; m[1][1] = y; m[1][2] = 0; m[1][3] = 0;
        m[2][0] = 0; m[2][1] = 0; m[2][2] = z; m[2][3] = 0;
        m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;

        return *this;
    }

    Matrix& make_scale(T uniformScale)
        requires (std::same_as<T, float>&& DIM_ROW == 4 && DIM_COL == 4) {
        return make_scale(uniformScale, uniformScale, uniformScale);
    }
    Matrix& make_scale(Vector3 const& vec)
        requires (std::same_as<T, float>&& DIM_ROW == 4 && DIM_COL == 4) {
        return make_scale(vec[0], vec[1], vec[2]);
    }

    Matrix& make_perspective(T front, T back, T fov)
        requires (std::same_as<T, float>&& DIM_ROW == 4 && DIM_COL == 4) {

        Matrix& m = *this;

        assert(fov > 0.0f);
        assert(front > 0.0f);
        assert(back > 0.0f);
        assert(front < back);

        //tan of the FOV
        T t = 1.0f / tan(fov / 2.0f);
        T a = -(back + front) / (back - front); //Col 2 Row 2
        T b = -(2.0f * front * back) / (back - front); //Col 3 Row 2

        //transposed for memory locality
        m[0][0] = t; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
        m[1][0] = 0; m[1][1] = t; m[1][2] = 0; m[1][3] = 0;
        m[2][0] = 0; m[2][1] = 0; m[2][2] = a; m[2][3] =-1;
        m[3][0] = 0; m[3][1] = 0; m[3][2] = b; m[3][3] = 0;

        return *this;
    }

    //STATIC CREATOR FUNCTIONS
    static Matrix& identity()
        requires (std::same_as<T, float>&& DIM_ROW == 4 && DIM_COL == 4) {
        Matrix matrix;
        return matrix.make_identity();
    }

    static Matrix& perspective(T front, T back, T fov)
        requires (std::same_as<T, float>&& DIM_ROW == 4 && DIM_COL == 4) {
        Matrix matrix;
        return matrix.make_perspective(front,back,fov);
    }


};

//Matrix defines
using Matrix4x4 = Matrix<float, 4, 4>;
using Matrix3x3 = Matrix<float, 3, 3>;