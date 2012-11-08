#define BUILDING_NODE_EXTENSION
#include <node.h>
#include "mpqtarchive.h"
#include "mpqtfile.h"


using namespace v8;

MPQTArchive::MPQTArchive() {};
MPQTArchive::~MPQTArchive() {};

Persistent<Function> MPQTArchive::constructor;

void MPQTArchive::Init() {
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("MPQTArchive"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  tpl->PrototypeTemplate()->Set(String::NewSymbol("openFile"), FunctionTemplate::New(Open)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("hasFile"), FunctionTemplate::New(HasFile)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("enumLocales"), FunctionTemplate::New(EnumLocales)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("createFile"), FunctionTemplate::New(Create)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("removeFile"), FunctionTemplate::New(Remove)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("renameFile"), FunctionTemplate::New(Rename)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("flush"), FunctionTemplate::New(Flush)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("closeArchive"), FunctionTemplate::New(Close)->GetFunction());

  constructor = Persistent<Function>::New(tpl->GetFunction());
}

Handle<Value> MPQTArchive::New(const Arguments& args) {
  HandleScope scope;

  MPQTArchive* obj = new MPQTArchive();
  obj->Wrap(args.This());

  return args.This();
}

Handle<Value> MPQTArchive::NewInstance(HANDLE hArchive) {
  HandleScope scope;

  Local<Object> instance = constructor->NewInstance();

  MPQTArchive* obj = ObjectWrap::Unwrap<MPQTArchive>(instance);
  obj->hArchive = hArchive;

  return scope.Close(instance);
}

Handle<Value> MPQTArchive::Open(const Arguments& args) {
  HandleScope scope;
  MPQTArchive* obj = ObjectWrap::Unwrap<MPQTArchive>(args.This());

  HANDLE hFile;

  String::Utf8Value str(args[0]);

  if (!SFileOpenFileEx( obj->hArchive, *str, SFILE_OPEN_FROM_MPQ, &hFile ))
  {
    fprintf(stderr, "%d", GetLastError());
    ThrowException(Exception::TypeError(String::New("There is no such file")));
    return scope.Close(Undefined());
  }

  return scope.Close(MPQTFile::NewInstance(hFile));
}

Handle<Value> MPQTArchive::HasFile(const Arguments& args) {
  HandleScope scope;
  MPQTArchive* obj = ObjectWrap::Unwrap<MPQTArchive>(args.This());

  String::Utf8Value filename(args[0]);

  bool is = SFileHasFile( obj->hArchive, *filename );

  return scope.Close( Boolean::New(is) );
}

Handle<Value> MPQTArchive::EnumLocales(const Arguments& args) {
  HandleScope scope;
  MPQTArchive* obj = ObjectWrap::Unwrap<MPQTArchive>(args.This());

  DWORD maxLocales = 10;
  LCID locales[maxLocales];

  String::Utf8Value filename(args[0]);

  SFileEnumLocales( obj->hArchive, *filename, locales, &maxLocales, NULL );

  Local<Array> array = Array::New(maxLocales);

  for (int index = 0; index<(int)maxLocales; index++) {
    array->Set(Integer::New(index), Integer::New( locales[index] ));
  }

  return scope.Close( array );
}

Handle<Value> MPQTArchive::Create(const Arguments& args) {
  HandleScope scope;
  MPQTArchive* obj = ObjectWrap::Unwrap<MPQTArchive>(args.This());

  HANDLE hFile;

  String::Utf8Value filename(args[0]);
  String::Utf8Value size(args[1]);

  if (!SFileCreateFile( obj->hArchive, *filename, 0, atoi( *size ), 0, NULL, &hFile ))
  {
    ThrowException(Exception::TypeError(String::New("Unable to create a file")));
    return scope.Close(Undefined());
  }

  return scope.Close(MPQTFile::NewInstance(hFile));
}

Handle<Value> MPQTArchive::Remove(const Arguments& args) {
  HandleScope scope;
  MPQTArchive* obj = ObjectWrap::Unwrap<MPQTArchive>(args.This());

  String::Utf8Value filename(args[0]);

  if (!SFileRemoveFile( obj->hArchive, *filename, NULL ))
  {
    ThrowException(Exception::TypeError(String::New("Unable to remove a file")));
    return scope.Close(Undefined());
  }

  return scope.Close( args.This() );
}

Handle<Value> MPQTArchive::Rename(const Arguments& args) {
  HandleScope scope;
  MPQTArchive* obj = ObjectWrap::Unwrap<MPQTArchive>(args.This());

  String::Utf8Value filename(args[0]);
  String::Utf8Value newFilename(args[1]);

  if (!SFileRenameFile( obj->hArchive, *filename, *newFilename ))
  {
    ThrowException(Exception::TypeError(String::New("Unable to rename a file")));
    return scope.Close(Undefined());
  }

  return scope.Close( args.This() );
}

Handle<Value> MPQTArchive::Flush(const Arguments& args) {
  HandleScope scope;
  MPQTArchive* obj = ObjectWrap::Unwrap<MPQTArchive>(args.This());

  String::Utf8Value filename(args[0]);
  String::Utf8Value newFilename(args[1]);

  if (!SFileFlushArchive( obj->hArchive ))
  {
    ThrowException(Exception::TypeError(String::New("Unable to flush")));
    return scope.Close(Undefined());
  }

  return scope.Close( args.This() );
}

Handle<Value> MPQTArchive::Close(const Arguments& args) {
  HandleScope scope;
  MPQTArchive* obj = ObjectWrap::Unwrap<MPQTArchive>(args.This());

  SFileCloseArchive( obj->hArchive );

  return scope.Close( args.This() );
}