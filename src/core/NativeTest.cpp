#include "NativeTest.h"
#include "Matrix.h"
#include "Rectangle.h"
#include "../utils/getTimer.h"
#include "iostream"

void runNative(const short* data, const int TIMES, const int SIZE) {

    auto rect = new Rectangle();
    auto matrix = new Matrix(2, 0, 0, 0.5, 11, 19);
    //Native测试
    auto start = getTimer();
    for (int t = 0; t < TIMES; t++) {
        int index = 0;
        for (int i = 0; i < SIZE; i++) {
            short x = data[index++];
            short y = data[index++];
            short width = data[index++];
            short height = data[index++];
            rect->setTo(x, y, width, height);
            matrix->transformBounds(rect);
        }
    }
    auto nativeTime = getTimer() - start;
    std::cout << "Native耗时:" << (int)nativeTime << "ms";
}