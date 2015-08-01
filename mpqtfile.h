#ifndef MPQTFILE_H
#define MPQTFILE_H

#include <node.h>
#include <node_object_wrap.h>
#include <nan.h>
#include "Stormlib/src/StormLib.h"

class MPQTFile : public node::ObjectWrap {
 public:
  static void Init();
  static v8::Handle<v8::Value> NewInstance(HANDLE hFile);

 private:
  MPQTFile();
  ~MPQTFile();

  static v8::Persistent<v8::Function> constructor;
  static NAN_METHOD(New);
  static NAN_METHOD(GetFileSize);
  static NAN_METHOD(Read);
  static NAN_METHOD(Write);
  static NAN_METHOD(Finish);
  static NAN_METHOD(Close);

  HANDLE hFile;
};

#endif
