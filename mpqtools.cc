#define BUILDING_NODE_EXTENSION
#include <node.h>
#include "mpqtarchive.h"
#include "mpqtfile.h"

using namespace v8;

Handle<Value> OpenArchive(const Arguments& args) {
  HandleScope scope;

  HANDLE hArchive;

  String::Utf8Value filename(args[0]);

  if (!SFileOpenArchive( *filename, 0, NULL, &hArchive ))
  {
  	fprintf(stderr, "%X", GetLastError());
    ThrowException(Exception::TypeError(String::New("Failed to open the file")));
    return scope.Close(Undefined());
  }

  return scope.Close(MPQTArchive::NewInstance(hArchive));
}

Handle<Value> CreateArchive(const Arguments& args) {
  HandleScope scope;

  HANDLE hArchive;

  String::Utf8Value filename(args[0]);

  if (!SFileCreateArchive( *filename, MPQ_CREATE_ARCHIVE_V2, 10, &hArchive ))
  {
    ThrowException(Exception::TypeError(String::New("Failed to create the file")));
    return scope.Close(Undefined());
  }

  return scope.Close(MPQTArchive::NewInstance(hArchive));
}

void InitAll(Handle<Object> target) {
  MPQTArchive::Init();
  MPQTFile::Init();

  target->Set(String::NewSymbol("openArchive"), FunctionTemplate::New(OpenArchive)->GetFunction());
  target->Set(String::NewSymbol("createArchive"), FunctionTemplate::New(CreateArchive)->GetFunction());

}

NODE_MODULE(mpqtools, InitAll)