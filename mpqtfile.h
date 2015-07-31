#ifndef MPQTFILE_H
#define MPQTFILE_H

#include <node.h>
#include "Stormlib/src/StormLib.h"

class MPQTFile : public node::ObjectWrap {
 public:
  static void Init();
  static v8::Handle<v8::Value> NewInstance(HANDLE hFile);

 private:
  MPQTFile();
  ~MPQTFile();

  static v8::Persistent<v8::Function> constructor;
  static v8::Handle<v8::Value> New(const v8::Arguments& args);
  static v8::Handle<v8::Value> GetFileSize(const v8::Arguments& args);
  static v8::Handle<v8::Value> Read(const v8::Arguments& args);
  static v8::Handle<v8::Value> Write(const v8::Arguments& args);
  static v8::Handle<v8::Value> Finish(const v8::Arguments& args);
  static v8::Handle<v8::Value> Close(const v8::Arguments& args);

  HANDLE hFile;
};

#endif
