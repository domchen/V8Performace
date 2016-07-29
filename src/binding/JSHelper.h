
#ifndef V8PERFORMANCE_JSHELPER_H
#define V8PERFORMANCE_JSHELPER_H

#include <v8.h>


#define JSString(text) v8::String::NewFromUtf8(isolate, text, v8::NewStringType::kNormal).ToLocalChecked()
#define JSFunction(callback) v8::Function::New(context,callback).ToLocalChecked()

#define GETTER_ARGUMENT v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info

#define GETTER_CONTENT(className, name) \
    auto self = info.Holder(); \
    auto ptr = self->GetAlignedPointerFromInternalField(0); \
    auto value = static_cast<className*>(ptr)->name; \
    info.GetReturnValue().Set(value);

#define SETTER_ARGUMENT v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info

#define SETTER_CONTENT(className, name) \
    auto self = info.Holder(); \
    auto isolate = info.GetIsolate(); \
    auto context = isolate->GetCurrentContext(); \
    auto ptr = self->GetAlignedPointerFromInternalField(0); \
    static_cast<className*>(ptr)->name = (float) value->NumberValue(context).FromJust();

#define SET_ACCESSOR(name, getter, setter) self->SetAccessor(context, JSString(name), getter, setter);

#define SET_METHOD(name, method) prototypeTemplate->Set(isolate, name, \
                                                            v8::FunctionTemplate::New(isolate, method));


#endif //V8PERFORMANCE_JSHELPER_H
