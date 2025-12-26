#pragma once
//Mathmatical vector
#include "defines.h"

#include "vector.h"
#include <algorithm>

template <typename T, unsigned int DIM_ROW, unsigned int DIM_COL>
class Matrix {

private:
    T data[DIM_ROW][DIM_COL]; //ROW MAJOR
    


public:

    Matrix() {}
    Matrix(Matrix<T, DIM_ROW, DIM_COL> const& other) = default;


    static Matrix Garbage() {
        return Matrix;
    }

    static Matrix Zero() {
        Matrix result;
        for (uint loop_row = 0; loop_row < DIM_ROW; loop_row++) {
            for (uint loop_col = 0; loop_col < DIM_COL; loop_col++) {
                result.data[loop_row][loop_col] = 0;
            }
        }
        return result;
    }
    
    static Matrix Identity() {
        Matrix result = Matrix::Zero();
        constexpr uint diagonal_min = std::min(DIM_ROW, DIM_COL);
        for (uint loop = 0; loop < diagonal_min; loop++) {
            result.data[loop][loop] = 1;
        }

        return result;
    }

    Matrix transpose() const {
        Matrix result;
        for (uint loop_row = 0; loop_row < DIM_ROW; loop_row++) {
            for (uint loop_col = 0; loop_col < DIM_COL; loop_col++) {
                result.data[loop_col][loop_row] = data[loop_row][loop_col];
            }
        }
        return result;
    }

    Vector<T, DIM_ROW> operator*(const Vector<T, DIM_COL>& vec) const {
        Vector<T, DIM_ROW> result;
        
        for (int loop_row = 0; loop_row < DIM_ROW; loop_row++) {
            result[loop_row] = T();
            for (int loop_col = 0; loop_col < DIM_COL; loop_col++) {
                result[loop_row] += data[loop_row][loop_col] * vec[loop_col];
            }
        }

        return result;
    }

    template <unsigned int OUTER_DIM>
    Matrix<T, DIM_ROW, OUTER_DIM> operator*(const Matrix<T, DIM_COL, OUTER_DIM>& rhs) const {
        Matrix<T, DIM_ROW, OUTER_DIM> result;

        for (int loop_row = 0; loop_row < DIM_ROW; loop_row++) {
            for (int loop_outer = 0; loop_outer < OUTER_DIM; loop_outer++) {
                result.data[loop_row][loop_outer] = T();
                for (int loop_col = 0; loop_col < DIM_COL; loop_col++) {
                    result.data[loop_row][loop_outer] += data[loop_row][loop_col] * rhs.data[loop_col][loop_outer];
                }
            }
        }

        return result;
    }

    T& operator() (uint row, uint col) {
        
        assert(row < DIM_ROW);
        assert(col < DIM_COL);

        return data[row][col];
    }

    T const& operator() (uint row, uint col) const {

        assert(row < DIM_ROW);
        assert(col < DIM_COL);

        return data[row][col];
    }

    static 
        Matrix<T,DIM_ROW,DIM_COL> CreateTranslation(T x, T y, T z) 
        requires (std::same_as<T, float> && DIM_ROW == 4 && DIM_COL == 4) {
        Matrix translation = Matrix::Identity();

        translation.data[0][3] = x;
        translation.data[1][3] = y;
        translation.data[2][3] = z;
        return translation;
    }

    static
        Matrix<T, DIM_ROW, DIM_COL> CreateTranslation(Vector3 vec)
        requires (std::same_as<T, float>&& DIM_ROW == 4 && DIM_COL == 4) {
        return CreateTranslation(vec[0], vec[1], vec[2]);
    }
    static
        Matrix<T, DIM_ROW, DIM_COL> CreateRotationX(T angle)
        requires (std::same_as<T, float>&& DIM_ROW == 4 && DIM_COL == 4) {
        Matrix rotation = Matrix::Identity();

        T cos_a = cos(angle);
        T sin_a = sin(angle);

        rotation.data[1][1] = cos_a;
        rotation.data[2][2] = cos_a;
        rotation.data[2][1] = sin_a;
        rotation.data[1][2] = -sin_a;

        return rotation;
    }

    static
        Matrix<T, DIM_ROW, DIM_COL> CreateRotationY(T angle)
        requires (std::same_as<T, float>&& DIM_ROW == 4 && DIM_COL == 4) {
        Matrix rotation = Matrix::Identity();

        T cos_a = cos(angle);
        T sin_a = sin(angle);

        rotation.data[0][0] = cos_a;
        rotation.data[2][2] = cos_a;
        rotation.data[2][0] = -sin_a;
        rotation.data[0][2] = sin_a;
        
        return rotation;
    }

    static
        Matrix<T, DIM_ROW, DIM_COL> CreateRotationZ(T angle)
        requires (std::same_as<T, float>&& DIM_ROW == 4 && DIM_COL == 4) {
        Matrix rotation = Matrix::Identity();

        T cos_a = cos(angle);
        T sin_a = sin(angle);

        rotation.data[0][0] = cos_a;
        rotation.data[1][1] = cos_a;
        rotation.data[0][1] = -sin_a;
        rotation.data[1][0] = sin_a;

        return rotation;
    }

    static
        Matrix<T, DIM_ROW, DIM_COL> CreateScale(T uniformScale)
        requires (std::same_as<T, float>&& DIM_ROW == 4 && DIM_COL == 4) {
        Matrix scale = Matrix::Identity();

        scale.data[0][0] = uniformScale;
        scale.data[1][1] = uniformScale;
        scale.data[2][2] = uniformScale;

        return scale;
    }

    static
        Matrix<T, DIM_ROW, DIM_COL> CreateScale(T scaleX, T scaleY, T scaleZ)
        requires (std::same_as<T, float>&& DIM_ROW == 4 && DIM_COL == 4) {
        Matrix scale = Matrix::Identity();

        scale.data[0][0] = scaleX;
        scale.data[1][1] = scaleY;
        scale.data[2][2] = scaleZ;

        return scale;
    }

    static
        Matrix<T, DIM_ROW, DIM_COL> CreateScale(Vector3 scaleVec)
        requires (std::same_as<T, float>&& DIM_ROW == 4 && DIM_COL == 4) {
       
        return CreateScale(scaleVec[0], scaleVec[1], scaleVec[2]);

    }



    static
        Matrix<T, DIM_ROW, DIM_COL>  CreatePerspective(T front, T back, T fov)
        requires (std::same_as<T, float>&& DIM_ROW == 4 && DIM_COL == 4) {
        Matrix<T, DIM_ROW, DIM_COL>  perspective = Matrix::Zero();

        assert(fov > 0.0f);
        assert(front > 0.0f);
        assert(back > 0.0f);
        assert(front < back);

        
        T tan_fov = 1.0f / tan(fov / 2.0f);

        perspective.data[0][0] = tan_fov;
        perspective.data[1][1] = tan_fov;
        perspective.data[2][2] = -(back + front) / (back - front);
        perspective.data[2][3] = -(2.0f * front * back) / (back - front);
        perspective.data[3][2] = -1.0f;

        return perspective;
    }
};

//Matrix defines
using Matrix4x4 = Matrix<float, 4, 4>;
using Matrix3x3 = Matrix<float, 3, 3>;


