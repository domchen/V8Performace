#include "Matrix.h"
#include "math.h"

Matrix::Matrix(float a, float b, float c, float d, float tx, float ty) {
    this->setTo(a, b, c, d, tx, ty);
}

void Matrix::setTo(float a, float b, float c, float d, float tx, float ty) {
    this->a = a;
    this->b = b;
    this->c = c;
    this->d = d;
    this->tx = tx;
    this->ty = ty;
}

void Matrix::transformBounds(Rectangle* bounds) {
    if (!bounds) {
        return;
    }
    auto a = this->a;
    auto b = this->b;
    auto c = this->c;
    auto d = this->d;
    auto tx = this->tx;
    auto ty = this->ty;

    auto x = bounds->x;
    auto y = bounds->y;
    auto xMax = x + bounds->width;
    auto yMax = y + bounds->height;

    auto x0 = a * x + c * y + tx;
    auto y0 = b * x + d * y + ty;
    auto x1 = a * xMax + c * y + tx;
    auto y1 = b * xMax + d * y + ty;
    auto x2 = a * xMax + c * yMax + tx;
    auto y2 = b * xMax + d * yMax + ty;
    auto x3 = a * x + c * yMax + tx;
    auto y3 = b * x + d * yMax + ty;

    float tmp = 0;

    if (x0 > x1) {
        tmp = x0;
        x0 = x1;
        x1 = tmp;
    }
    if (x2 > x3) {
        tmp = x2;
        x2 = x3;
        x3 = tmp;
    }

    bounds->x = floorf(x0 < x2 ? x0 : x2);
    bounds->width = ceilf((x1 > x3 ? x1 : x3) - bounds->x);

    if (y0 > y1) {
        tmp = y0;
        y0 = y1;
        y1 = tmp;
    }
    if (y2 > y3) {
        tmp = y2;
        y2 = y3;
        y3 = tmp;
    }

    bounds->y = floorf(y0 < y2 ? y0 : y2);
    bounds->height = ceilf((y1 > y3 ? y1 : y3) - bounds->y);
}

std::string Matrix::toString() {
    return "(a=" + std::to_string(this->a) + ", b=" + std::to_string(this->b) + ", c=" + std::to_string(this->c) +
           ", d=" + std::to_string(this->d) + ", tx=" + std::to_string(this->tx) + ", ty=" + std::to_string(this->ty) +
           ")";
}

void Matrix::transformBoundsNoField(float x, float y, float width, float height) {
    float a = 2;
    float b = 0;
    float c = 0;
    float d = 0.5;
    float tx = 11;
    float ty = 19;

    float xMax = x + width;
    float yMax = y + height;

    float x0 = a * x + c * y + tx;
    float y0 = b * x + d * y + ty;
    float x1 = a * xMax + c * y + tx;
    float y1 = b * xMax + d * y + ty;
    float x2 = a * xMax + c * yMax + tx;
    float y2 = b * xMax + d * yMax + ty;
    float x3 = a * x + c * yMax + tx;
    float y3 = b * x + d * yMax + ty;

    float tmp = 0;

    if (x0 > x1) {
        tmp = x0;
        x0 = x1;
        x1 = tmp;
    }
    if (x2 > x3) {
        tmp = x2;
        x2 = x3;
        x3 = tmp;
    }

    x = floorf(x0 < x2 ? x0 : x2);
    width = ceilf((x1 > x3 ? x1 : x3) - x);

    if (y0 > y1) {
        tmp = y0;
        y0 = y1;
        y1 = tmp;
    }
    if (y2 > y3) {
        tmp = y2;
        y2 = y3;
        y3 = tmp;
    }

    y = floorf(y0 < y2 ? y0 : y2);
    height = ceilf((y1 > y3 ? y1 : y3) - y);
}