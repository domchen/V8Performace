//////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2014-present, Egret Technology.
//  All rights reserved.
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the Egret nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY EGRET AND CONTRIBUTORS "AS IS" AND ANY EXPRESS
//  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
//  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
//  IN NO EVENT SHALL EGRET AND CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;LOSS OF USE, DATA,
//  OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
//  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//////////////////////////////////////////////////////////////////////////////////////

#ifndef ELVEN_JSHELPER_H
#define ELVEN_JSHELPER_H

#include <v8.h>

namespace elven {

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


}  // namespace elven

#endif //ELVEN_JSHELPER_H
