#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libplatform/libplatform.h"
#include "v8.h"
#include "binding/V8Console.h"
#include "binding/V8Matrix.h"
#include "binding/V8Rectangle.h"
#include "utils/File.h"


using namespace v8;

class ArrayBufferAllocator : public v8::ArrayBuffer::Allocator {
public:
    virtual void* Allocate(size_t length) {
        void* data = AllocateUninitialized(length);
        return data == NULL ? data : memset(data, 0, length);
    }

    virtual void* AllocateUninitialized(size_t length) { return malloc(length); }

    virtual void Free(void* data, size_t) { free(data); }
};


int main(int argc, char* argv[]) {
    // Initialize V8.
    V8::InitializeICU();
    Platform* platform = platform::CreateDefaultPlatform();
    V8::InitializePlatform(platform);
    V8::Initialize();

    // Create a new Isolate and make it the current one.
    ArrayBufferAllocator allocator;
    Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = &allocator;
    Isolate* isolate = Isolate::New(create_params);
    {
        Isolate::Scope isolate_scope(isolate);

        // Create a stack-allocated handle scope.
        HandleScope handle_scope(isolate);

        Local<ObjectTemplate> global = ObjectTemplate::New(isolate);
        V8Console::install(isolate, global);
        V8Rectangle::install(isolate, global);
        V8Matrix::install(isolate, global);

        // Create a new context.
        Local<Context> context = Context::New(isolate, nullptr, global);

        // Enter the context for compiling and running the hello world script.
        Context::Scope context_scope(context);

        auto text = File::read("JSTest.js");
        // Create a string containing the JavaScript source code.
        Local<String> source =
                String::NewFromUtf8(isolate, text.c_str(),
                                    NewStringType::kNormal).ToLocalChecked();

        // Compile the source code.
        Local<Script> script = Script::Compile(context, source).ToLocalChecked();

        TryCatch trycatch(isolate);
        // Run the script to get the result.
        Local<Value> result = script->Run(context).ToLocalChecked();

        if (result.IsEmpty()) {
            Local<Value> exception = trycatch.Exception();
            String::Utf8Value exception_str(exception);
            printf("Exception: %s\n", *exception_str);
        }
    }

    // Dispose the isolate and tear down V8.
    isolate->Dispose();
    V8::Dispose();
    V8::ShutdownPlatform();
    delete platform;
    return 0;
}
