#include <node.h>
#include "mpqtarchive.h"
#include "mpqtfile.h"

using namespace v8;

MPQTArchive::MPQTArchive() {};
MPQTArchive::~MPQTArchive() {};

Persistent<Function> MPQTArchive::constructor;

void MPQTArchive::Init() {
  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->SetClassName(NanNew("MPQTArchive"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  tpl->PrototypeTemplate()->Set(NanNew("openFile"), NanNew<FunctionTemplate>(Open)->GetFunction());
  tpl->PrototypeTemplate()->Set(NanNew("hasFile"), NanNew<FunctionTemplate>(HasFile)->GetFunction());
  tpl->PrototypeTemplate()->Set(NanNew("enumLocales"), NanNew<FunctionTemplate>(EnumLocales)->GetFunction());
  tpl->PrototypeTemplate()->Set(NanNew("createFile"), NanNew<FunctionTemplate>(Create)->GetFunction());
  tpl->PrototypeTemplate()->Set(NanNew("removeFile"), NanNew<FunctionTemplate>(Remove)->GetFunction());
  tpl->PrototypeTemplate()->Set(NanNew("renameFile"), NanNew<FunctionTemplate>(Rename)->GetFunction());
  tpl->PrototypeTemplate()->Set(NanNew("flush"), NanNew<FunctionTemplate>(Flush)->GetFunction());
  tpl->PrototypeTemplate()->Set(NanNew("closeArchive"), NanNew<FunctionTemplate>(Close)->GetFunction());

  NanAssignPersistent(constructor, tpl->GetFunction());
}

NAN_METHOD(MPQTArchive::New) {
  NanScope();

  MPQTArchive* obj = new MPQTArchive();
  obj->Wrap(args.This());

  NanReturnValue(args.This());
}

Handle<Value> MPQTArchive::NewInstance(HANDLE hArchive) {
  NanEscapableScope();

  Local<Function> cons = NanNew<Function>(constructor);
  Local<Object> instance = cons->NewInstance();

  MPQTArchive* obj = ObjectWrap::Unwrap<MPQTArchive>(instance);
  obj->hArchive = hArchive;

  return NanEscapeScope(instance);
}

NAN_METHOD(MPQTArchive::Open) {
  NanScope();
  MPQTArchive* obj = ObjectWrap::Unwrap<MPQTArchive>(args.This());

  HANDLE hFile;

  String::Utf8Value str(args[0]);

  if (!SFileOpenFileEx( obj->hArchive, *str, SFILE_OPEN_FROM_MPQ, &hFile ))
  {
    fprintf(stderr, "%d", GetLastError());
    NanThrowError("There is no such file");
    NanReturnUndefined();
  }

  NanReturnValue(MPQTFile::NewInstance(hFile));
}

NAN_METHOD(MPQTArchive::HasFile) {
  NanScope();
  MPQTArchive* obj = ObjectWrap::Unwrap<MPQTArchive>(args.This());

  String::Utf8Value filename(args[0]);

  bool is = SFileHasFile( obj->hArchive, *filename );

  NanReturnValue(NanNew<Boolean>(is));
}

NAN_METHOD(MPQTArchive::EnumLocales) {
  NanScope();
  MPQTArchive* obj = ObjectWrap::Unwrap<MPQTArchive>(args.This());

  DWORD maxLocales = 10;
  LCID locales[maxLocales];

  String::Utf8Value filename(args[0]);

  SFileEnumLocales( obj->hArchive, *filename, locales, &maxLocales, 0 );

  Local<Array> array = NanNew<Array>(maxLocales);

  for (int index = 0; index<(int)maxLocales; index++) {
    array->Set(NanNew<Integer>(index), NanNew<Integer>( locales[index] ));
  }

  NanReturnValue(array);
}

NAN_METHOD(MPQTArchive::Create) {
  NanScope();
  MPQTArchive* obj = ObjectWrap::Unwrap<MPQTArchive>(args.This());

  HANDLE hFile;

  String::Utf8Value filename(args[0]);
  String::Utf8Value size(args[1]);

  if (!SFileCreateFile( obj->hArchive, *filename, 0, atoi( *size ), 0, 0x0, &hFile ))
  {
    NanThrowError("Unable to create a file");
    NanReturnUndefined();
  }

  NanReturnValue(MPQTFile::NewInstance(hFile));
}

NAN_METHOD(MPQTArchive::Remove) {
  NanScope();
  MPQTArchive* obj = ObjectWrap::Unwrap<MPQTArchive>(args.This());

  String::Utf8Value filename(args[0]);

  if (!SFileRemoveFile( obj->hArchive, *filename, 0 ))
  {
    NanThrowError("Unable to remove a file");
    NanReturnUndefined();
  }

  NanReturnValue(args.This());
}

NAN_METHOD(MPQTArchive::Rename) {
  NanScope();
  MPQTArchive* obj = ObjectWrap::Unwrap<MPQTArchive>(args.This());

  String::Utf8Value filename(args[0]);
  String::Utf8Value newFilename(args[1]);

  if (!SFileRenameFile( obj->hArchive, *filename, *newFilename ))
  {
    NanThrowError("Unable to rename a file");
    NanReturnUndefined();
  }

  NanReturnValue(args.This());
}

NAN_METHOD(MPQTArchive::Flush) {
  NanScope();
  MPQTArchive* obj = ObjectWrap::Unwrap<MPQTArchive>(args.This());

  String::Utf8Value filename(args[0]);
  String::Utf8Value newFilename(args[1]);

  if (!SFileFlushArchive( obj->hArchive ))
  {
    NanThrowError("Unable to flush");
    NanReturnUndefined();
  }

  NanReturnValue(args.This());
}

NAN_METHOD(MPQTArchive::Close) {
  NanScope();
  MPQTArchive* obj = ObjectWrap::Unwrap<MPQTArchive>(args.This());

  SFileCloseArchive( obj->hArchive );

  NanReturnValue(args.This());
}
