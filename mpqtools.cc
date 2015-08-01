#include <node.h>
#include <nan.h>
#include "mpqtarchive.h"
#include "mpqtfile.h"

using namespace v8;

NAN_METHOD(OpenArchive) {
  NanScope();

  HANDLE hArchive;

  String::Utf8Value filename(args[0]);

  if (!SFileOpenArchive( *filename, 0, NULL, &hArchive ))
  {
    fprintf(stderr, "%X", GetLastError());
    NanThrowError("Failed to open the file");
    NanReturnUndefined();
  }

  NanReturnValue(MPQTArchive::NewInstance(hArchive));
}

NAN_METHOD(CreateArchive) {
  NanScope();

  HANDLE hArchive;

  String::Utf8Value filename(args[0]);

  if (!SFileCreateArchive( *filename, MPQ_CREATE_ARCHIVE_V2, 10, &hArchive ))
  {
    NanThrowError("Failed to create the file");
    NanReturnUndefined();
  }

  NanReturnValue(MPQTArchive::NewInstance(hArchive));
}

void InitAll(Handle<Object> target) {
  MPQTArchive::Init();
  MPQTFile::Init();

  target->Set(NanNew("openArchive"), NanNew<FunctionTemplate>(OpenArchive)->GetFunction());
  target->Set(NanNew("createArchive"), NanNew<FunctionTemplate>(CreateArchive)->GetFunction());

}

NODE_MODULE(mpqtools, InitAll)
