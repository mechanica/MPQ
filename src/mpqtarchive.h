#ifndef MPQTARCHIVE_H
#define MPQTARCHIVE_H

#include <node.h>
#include <nan.h>
#include "Stormlib/src/StormLib.h"

class MPQTArchive : public node::ObjectWrap {
 public:
  static void Init();
  static v8::Handle<v8::Value> NewInstance(HANDLE hArchive);

 private:
  MPQTArchive();
  ~MPQTArchive();

  static v8::Persistent<v8::Function> constructor;
  static NAN_METHOD(New);
  static NAN_METHOD(Open);
  static NAN_METHOD(HasFile);
  static NAN_METHOD(EnumLocales);
  static NAN_METHOD(Create);
  static NAN_METHOD(Remove);
  static NAN_METHOD(Rename);
  static NAN_METHOD(Flush);
  static NAN_METHOD(Close);
  HANDLE hArchive;
};

#endif
