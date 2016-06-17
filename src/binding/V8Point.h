#ifndef V8PERFORMANCE_V8POINT_H
#define V8PERFORMANCE_V8POINT_H

#include "v8.h"

class V8Point {
public:
    static void install(v8::Isolate* isolate, v8::Local<v8::ObjectTemplate> parent);

    static void runTest(short* data, int length, int j);
};


#endif //V8PERFORMANCE_V8POINT_H
