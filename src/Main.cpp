#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libplatform/libplatform.h"
#include "v8.h"
#include "binding/V8Console.h"
#include "binding/V8Matrix.h"
#include "binding/V8Rectangle.h"
#include "utils/File.h"
#include "debug/DebugAgent.h"

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
    V8::InitializeExternalStartupData("natives_blob.bin", "snapshot_blob.bin");
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

        std::string path = argv[0];
        std::replace(path.begin(), path.end(), '\\', '/');
        auto index = path.rfind("/");
        std::string currentPath = path.substr(0, index + 1);

        // Enable the debug angent.
        std::string arg = argc > 1 ? argv[1] : "";
        bool waitForConnection = (arg == "--debug-brk");
        DebugAgent::Enable("Elven", 5959, waitForConnection);

        auto jsPath = currentPath + "JSTest.js";
        auto text = File::read(jsPath.c_str());
        auto source = v8::String::NewFromUtf8(isolate, text.c_str(),
                                              v8::NewStringType::kNormal).ToLocalChecked();
        v8::ScriptOrigin origin(
                v8::String::NewFromUtf8(isolate, jsPath.c_str(),
                                        v8::NewStringType::kNormal).ToLocalChecked());
        TryCatch tryCatch(isolate);
        auto script = v8::Script::Compile(context, source, &origin);
        if (script.IsEmpty()) {
            Local<Value> exception = tryCatch.Exception();
            String::Utf8Value exception_str(exception);
            printf("Exception: %s\n", *exception_str);
        }
        else {
            // Run the script to get the result.
            Local<Value> result = script.ToLocalChecked()->Run(context).ToLocalChecked();

            if (result.IsEmpty()) {
                Local<Value> exception = tryCatch.Exception();
                String::Utf8Value exception_str(exception);
                printf("Exception: %s\n", *exception_str);
            }
        }

    }

    // Dispose the isolate and tear down V8.
    isolate->Dispose();
    V8::Dispose();
    V8::ShutdownPlatform();
    delete platform;
    return 0;
}
