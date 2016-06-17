#include "V8Console.h"
#include "../core/NativeTest.h"

namespace {

    void logMethod(const v8::FunctionCallbackInfo<v8::Value>& args) {
        v8::String::Utf8Value utf8(args[0]);
        printf("%s\n", *utf8);
    }

    void runtNativeTestMethod(const v8::FunctionCallbackInfo<v8::Value>& args) {
        auto times = (int) args[1]->NumberValue();
        auto array = v8::Local<v8::Int16Array>::Cast(args[0]);
        int size = int(array->Length() / 4);
        auto content = array->Buffer()->GetContents();
        auto data = (short*) content.Data();
        runNative(data, times, size);
    }
}

void V8Console::install(v8::Isolate* isolate, v8::Local<v8::ObjectTemplate> parent) {
    auto console = v8::ObjectTemplate::New(isolate);
    console->Set(v8::String::NewFromUtf8(isolate, "log"), v8::FunctionTemplate::New(isolate, logMethod));
    parent->Set(v8::String::NewFromUtf8(isolate, "runtNativeTest"),
                v8::FunctionTemplate::New(isolate, runtNativeTestMethod));
    parent->Set(v8::String::NewFromUtf8(isolate, "console"), console);
}