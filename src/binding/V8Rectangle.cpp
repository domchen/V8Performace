#include "V8Rectangle.h"
#include "../core/Rectangle.h"

namespace {

#define GETTER_ARGUMENT v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info

#define GETTER_CONTENT(className, name) \
    auto self = info.Holder(); \
    auto ptr = self->GetAlignedPointerFromInternalField(0); \
    auto value = static_cast<className*>(ptr)->name; \
    info.GetReturnValue().Set(value);

#define SETTER_ARGUMENT v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info

#define SETTER_CONTENT(className, name) \
    auto self = info.Holder(); \
    auto ptr = self->GetAlignedPointerFromInternalField(0); \
    static_cast<className*>(ptr)->name = (float) value->NumberValue();

#define SET_ACCESSOR(name, getter, setter) self->SetAccessor(v8::String::NewFromUtf8(isolate, name), getter, setter);

#define SET_METHOD(name, method) prototypeTemplate->Set(v8::String::NewFromUtf8(isolate, name), \
                                                            v8::FunctionTemplate::New(isolate, method));

    void xGetter(GETTER_ARGUMENT) { GETTER_CONTENT(Rectangle, x); }

    void xSetter(SETTER_ARGUMENT) { SETTER_CONTENT(Rectangle, x); }

    void yGetter(GETTER_ARGUMENT) { GETTER_CONTENT(Rectangle, y); }

    void ySetter(SETTER_ARGUMENT) { SETTER_CONTENT(Rectangle, y); }

    void widthGetter(GETTER_ARGUMENT) { GETTER_CONTENT(Rectangle, width); }

    void widthSetter(SETTER_ARGUMENT) { SETTER_CONTENT(Rectangle, width); }

    void heightGetter(GETTER_ARGUMENT) { GETTER_CONTENT(Rectangle, height); }

    void heightSetter(SETTER_ARGUMENT) { SETTER_CONTENT(Rectangle, height); }


    void setToMethod(const v8::FunctionCallbackInfo<v8::Value>& args) {
        float x = float(args[0]->IsUndefined() ? 0 : args[0]->NumberValue());
        float y = float(args[1]->IsUndefined() ? 0 : args[1]->NumberValue());
        float width = float(args[2]->IsUndefined() ? 0 : args[2]->NumberValue());
        float height = float(args[3]->IsUndefined() ? 0 : args[3]->NumberValue());
        auto self = args.Holder();
        auto ptr = self->GetAlignedPointerFromInternalField(0);
        static_cast<Rectangle*>(ptr)->setTo(x, y, width, height);
    }

    void toStringMethod(const v8::FunctionCallbackInfo<v8::Value>& args) {
        auto self = args.Holder();
        auto rect = static_cast<Rectangle*>(self->GetAlignedPointerFromInternalField(0));
        auto value = rect->toString();
        auto utf = v8::String::NewFromUtf8(args.GetIsolate(), value.c_str());
        args.GetReturnValue().Set(utf);
    }

    void constructor(const v8::FunctionCallbackInfo<v8::Value>& args) {
        float x = float(args[0]->IsUndefined() ? 0 : args[0]->NumberValue());
        float y = float(args[1]->IsUndefined() ? 0 : args[1]->NumberValue());
        float width = float(args[2]->IsUndefined() ? 0 : args[2]->NumberValue());
        float height = float(args[3]->IsUndefined() ? 0 : args[3]->NumberValue());
        Rectangle* rect = new Rectangle(x, y, width, height);
        auto self = args.Holder();
        self->SetAlignedPointerInInternalField(0, rect);
        auto isolate = args.GetIsolate();
        SET_ACCESSOR("x", xGetter, xSetter);
        SET_ACCESSOR("y", yGetter, ySetter);
        SET_ACCESSOR("width", widthGetter, widthSetter);
        SET_ACCESSOR("height", heightGetter, heightSetter);
        args.GetReturnValue().Set(self);
    }


}

void V8Rectangle::install(v8::Isolate* isolate, v8::Local<v8::ObjectTemplate> parent) {
    auto classTemplate = v8::FunctionTemplate::New(isolate, constructor);
    classTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "Rectangle"));
    auto prototypeTemplate = classTemplate->PrototypeTemplate();
    SET_METHOD("setTo", setToMethod);
    SET_METHOD("toString", toStringMethod);
    classTemplate->InstanceTemplate()->SetInternalFieldCount(1);

    parent->Set(v8::String::NewFromUtf8(isolate, "Rectangle"), classTemplate);
}
