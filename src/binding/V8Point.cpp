#include "V8Point.h"
#include "math.h"
#include "../geom/Point.h";

namespace {

    static void xGetter(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
        auto self = info.Holder();
        auto wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
        void* ptr = wrap->Value();
        float value = static_cast<Point*>(ptr)->x;
        info.GetReturnValue().Set(value);
    }

    static void xSetter(v8::Local<v8::Name> property, v8::Local<v8::Value> value,
                        const v8::PropertyCallbackInfo<void>& info) {
        auto self = info.Holder();
        auto wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
        void* ptr = wrap->Value();
        static_cast<Point*>(ptr)->x = (float) value->NumberValue();
    }

    static void yGetter(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
        auto self = info.Holder();
        auto wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
        void* ptr = wrap->Value();
        float value = static_cast<Point*>(ptr)->y;
        info.GetReturnValue().Set(value);
    }

    static void ySetter(v8::Local<v8::Name> property, v8::Local<v8::Value> value,
                        const v8::PropertyCallbackInfo<void>& info) {
        auto self = info.Holder();
        auto wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
        void* ptr = wrap->Value();
        static_cast<Point*>(ptr)->y = (float) value->NumberValue();
    }

    static void setTo(double x, double y, double width, double height) {
        double a = 2;
        double b = 0;
        double c = 0;
        double d = 0.5;
        double tx = 100;
        double ty = 50;
        double xMax = x + width;
        double yMax = y + height;
        double x0 = a * x + c * y + tx;
        double y0 = b * x + d * y + ty;
        double x1 = a * xMax + c * y + tx;
        double y1 = b * xMax + d * y + ty;
        double x2 = a * xMax + c * yMax + tx;
        double y2 = b * xMax + d * yMax + ty;
        double x3 = a * x + c * yMax + tx;
        double y3 = b * x + d * yMax + ty;
        double tmp = 0;
        if (x0 > x1) {
            tmp = x0;
            x0 = x1;
            x1 = tmp;
        }
        if (x2 > x3) {
            tmp = x2;
            x2 = x3;
            x3 = tmp;
        }
        x = floor(x0 < x2 ? x0 : x2);
        width = ceil((x1 > x3 ? x1 : x3) - x);
        if (y0 > y1) {
            tmp = y0;
            y0 = y1;
            y1 = tmp;
        }
        if (y2 > y3) {
            tmp = y2;
            y2 = y3;
            y3 = tmp;
        }
        y = floor(y0 < y2 ? y0 : y2);
        height = ceil((y1 > y3 ? y1 : y3) - y);
    }

    static void setToMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
//        double x = info[0]->IsUndefined() ? 0 : info[0]->NumberValue();
//        double y = info[1]->IsUndefined() ? 0 : info[1]->NumberValue();
//        static_cast<SkPoint*>(ptr)->set((SkScalar) x, (SkScalar) y);
        double x = info[0]->NumberValue();
        double y = info[1]->NumberValue();
        double width = info[2]->NumberValue();
        double height = info[3]->NumberValue();
        setTo(x, y, width, height);
    }

    static void setTo2Method(const v8::FunctionCallbackInfo<v8::Value>& info) {
        auto array = v8::Local<v8::Int16Array>::Cast(info[0]);
        auto length = int(array->Length());
        auto content = array->Buffer()->GetContents();
        short* data = (short*) content.Data();
        V8Point::runTest(data, length / 4, 0);
    }

    static void setTo3Method(const v8::FunctionCallbackInfo<v8::Value>& info) {
        auto array = v8::Local<v8::Int16Array>::Cast(info[0]);
        int index = int(info[1]->NumberValue()*4);
        auto length = int(array->Length());
        auto content = array->Buffer()->GetContents();
        short* data = (short*) content.Data();

        double x = data[index++];
        double y = data[index++];
        double width = data[index++];
        double height = data[index++];
        setTo(x, y, width, height);
    }

    static void constructor(const v8::FunctionCallbackInfo<v8::Value>& info) {
        double x = info[0]->IsUndefined() ? 0 : info[0]->NumberValue();
        double y = info[1]->IsUndefined() ? 0 : info[1]->NumberValue();
        Point* point = new Point();
        point->setTo((float) x, (float) y);
        auto self = info.Holder();
        auto isolate = info.GetIsolate();
        self->SetInternalField(0, v8::External::New(isolate, point));
        self->SetAccessor(v8::String::NewFromUtf8(isolate, "x"), xGetter, xSetter);
        self->SetAccessor(v8::String::NewFromUtf8(isolate, "y"), yGetter, ySetter);
        info.GetReturnValue().Set(self);
    }


}

void V8Point::install(v8::Isolate* isolate, v8::Local<v8::ObjectTemplate> parent) {
    auto classTemplate = v8::FunctionTemplate::New(isolate, constructor);
    classTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "Point"));
    auto prototypeTemplate = classTemplate->PrototypeTemplate();
    prototypeTemplate->Set(v8::String::NewFromUtf8(isolate, "setTo"), v8::FunctionTemplate::New(isolate, setToMethod));
    prototypeTemplate->Set(v8::String::NewFromUtf8(isolate, "setTo2"),
                           v8::FunctionTemplate::New(isolate, setTo2Method));
    prototypeTemplate->Set(v8::String::NewFromUtf8(isolate, "setTo3"),
                           v8::FunctionTemplate::New(isolate, setTo3Method));
    classTemplate->InstanceTemplate()->SetInternalFieldCount(1);

    parent->Set(v8::String::NewFromUtf8(isolate, "Point"), classTemplate);
}

void V8Point::runTest(short* data, int length, int j) {
    int index = 0;
    for (int i = 0; i < length; i++) {
        int x = data[index++];
        int y = data[index++];
        int width = data[index++];
        int height = data[index++];
        setTo(x, y, width, height);
    }
}