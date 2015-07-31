#ifndef MPQTARCHIVE_H
#define MPQTARCHIVE_H

#include <node.h>
#include "Stormlib/src/StormLib.h"

class MPQTArchive : public node::ObjectWrap {
 public:
  static void Init();
  static v8::Handle<v8::Value> NewInstance(HANDLE hArchive);

 private:
  MPQTArchive();
  ~MPQTArchive();

  static v8::Persistent<v8::Function> constructor;
  static v8::Handle<v8::Value> New(const v8::Arguments& args);
  static v8::Handle<v8::Value> Open(const v8::Arguments& args);
  static v8::Handle<v8::Value> HasFile(const v8::Arguments& args);
  static v8::Handle<v8::Value> EnumLocales(const v8::Arguments& args);
  static v8::Handle<v8::Value> Create(const v8::Arguments& args);
  static v8::Handle<v8::Value> Remove(const v8::Arguments& args);
  static v8::Handle<v8::Value> Rename(const v8::Arguments& args);
  static v8::Handle<v8::Value> Flush(const v8::Arguments& args);
  static v8::Handle<v8::Value> Close(const v8::Arguments& args);
  HANDLE hArchive;
};

#endif
