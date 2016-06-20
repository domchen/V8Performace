#include "Rectangle.h"

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
    return "(x=" + std::to_string(this->x) + ", y=" + std::to_string(this->y) +
           ", width=" + std::to_string(this->width) + ", height=" + std::to_string(this->height) + ")";
}