#include "V8Console.h"

namespace {

    void logMethod(const v8::FunctionCallbackInfo<v8::Value>& args) {
        v8::String::Utf8Value utf8(args[0]);
        printf("%s\n", *utf8);
    }
}

void V8Console::install(v8::Isolate* isolate, v8::Local<v8::ObjectTemplate> parent) {
    auto console = v8::ObjectTemplate::New(isolate);
    console->Set(v8::String::NewFromUtf8(isolate, "log"), v8::FunctionTemplate::New(isolate, logMethod));
    parent->Set(v8::String::NewFromUtf8(isolate, "console"), console);
}