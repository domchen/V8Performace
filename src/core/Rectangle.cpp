#include "Rectangle.h"
#include "sstream"

Rectangle::Rectangle(float x, float y, float width, float height) {
    this->setTo(x, y, width, height);
}

void Rectangle::setTo(float x, float y, float width, float height) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

std::string Rectangle::toString() {
    std::stringstream ss;
    ss << "(x="<<this->x<<", y="<<this->y<<", width="<<this->width<<", height="<<this->height<<")";
    return ss.str();
}