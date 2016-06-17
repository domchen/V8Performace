
#ifndef V8PERFORMANCE_V8CONSOLE_H
#define V8PERFORMANCE_V8CONSOLE_H

#include "v8.h"

class V8Console {
public:
    static void install(v8::Isolate* isolate, v8::Local<v8::ObjectTemplate> parent);
};


#endif //V8PERFORMANCE_V8CONSOLE_H
