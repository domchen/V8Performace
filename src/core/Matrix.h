#ifndef V8PERFORMANCE_MATRIX_H
#define V8PERFORMANCE_MATRIX_H

#include "string"
#include "Rectangle.h"

class Matrix {
public:
    Matrix(float a = 1, float b = 0, float c = 0, float d = 1, float tx = 0, float ty = 0);

    /**
     * 将 Matrix 的成员设置为指定值
     * @param a 缩放或旋转图像时影响像素沿 x 轴定位的值。
     * @param b 旋转或倾斜图像时影响像素沿 y 轴定位的值。
     * @param c 旋转或倾斜图像时影响像素沿 x 轴定位的值。
     * @param d 缩放或旋转图像时影响像素沿 y 轴定位的值。
     * @param tx 沿 x 轴平移每个点的距离。
     * @param ty 沿 y 轴平移每个点的距离。
     */
    void setTo(float a, float b, float c, float d, float tx, float ty);

    void transformBounds(Rectangle* bounds);

    std::string toString();

    float a, b, c, d, tx, ty;
};


#endif //V8PERFORMANCE_MATRIX_H
