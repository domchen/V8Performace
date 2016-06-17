#include "Matrix.h"
#include "math.h"
#include <sstream>

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
    std::stringstream ss;
    ss << "(a="<<this->a<<", b="<<this->b<<", c="<<this->c<<", d="<<this->d<<", tx="<<this->tx<<", ty="<<this->ty<<")";
    return ss.str();
}