#ifndef MPQARCHIVE_H
#define MPQARCHIVE_H

#include <node.h>
#include <nan.h>
#include "Stormlib/src/StormLib.h"

class MPQArchive : public Nan::ObjectWrap {
public:
  static void Init();
  static v8::Local<v8::Value> NewInstance(HANDLE handle);

private:
  MPQArchive();
  ~MPQArchive() {}

  static Nan::Persistent<v8::Function> constructor;
  static NAN_METHOD(New);
  static NAN_METHOD(OpenFile);
  static NAN_METHOD(HasFile);
  static NAN_METHOD(CreateFile);
  static NAN_METHOD(RemoveFile);
  static NAN_METHOD(RenameFile);
  static NAN_METHOD(Flush);
  static NAN_METHOD(Close);

  HANDLE _handle;
};

#endif
