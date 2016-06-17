#ifndef V8PERFORMANCE_RECTANGLE_H
#define V8PERFORMANCE_RECTANGLE_H

#include "string"

class Rectangle {
public:
    Rectangle(float x = 0, float y = 0, float width = 0, float height = 0);

    void setTo(float x, float y, float width, float height);

    std::string toString();

    float x, y, width, height;
};


#endif //V8PERFORMANCE_RECTANGLE_H
