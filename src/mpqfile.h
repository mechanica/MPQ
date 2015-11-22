#ifndef MPQFILE_H
#define MPQFILE_H

#include <node.h>
#include <nan.h>
#include "Stormlib/src/StormLib.h"

class MPQFile : public Nan::ObjectWrap {
public:
  static void Init();
  static v8::Local<v8::Value> NewInstance(HANDLE handle);

private:
  MPQFile();
  ~MPQFile() {}

  static Nan::Persistent<v8::Function> constructor;
  static NAN_METHOD(New);
  static NAN_METHOD(GetSize);
  static NAN_METHOD(Read);
  static NAN_METHOD(Write);
  static NAN_METHOD(Finish);
  static NAN_METHOD(Close);

  HANDLE _handle;
};

#endif
