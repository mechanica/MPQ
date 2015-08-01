#include <node.h>
#include <node_buffer.h>
#include "mpqtfile.h"

using namespace v8;

static Local<Object> makeBuffer(char* data, size_t size) {
  NanEscapableScope();

  Local<Object> buffer = NanNewBufferHandle(data, size);
  return NanEscapeScope(buffer);
}

MPQTFile::MPQTFile() {};
MPQTFile::~MPQTFile() {};

Persistent<Function> MPQTFile::constructor;

void MPQTFile::Init() {
  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->SetClassName(NanNew("MPQTFile"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  tpl->PrototypeTemplate()->Set(NanNew("getFileSize"), NanNew<FunctionTemplate>(GetFileSize)->GetFunction());
  tpl->PrototypeTemplate()->Set(NanNew("readFile"), NanNew<FunctionTemplate>(Read)->GetFunction());
  tpl->PrototypeTemplate()->Set(NanNew("writeFile"), NanNew<FunctionTemplate>(Write)->GetFunction());
  tpl->PrototypeTemplate()->Set(NanNew("finishFile"), NanNew<FunctionTemplate>(Finish)->GetFunction());
  tpl->PrototypeTemplate()->Set(NanNew("closeFile"), NanNew<FunctionTemplate>(Close)->GetFunction());

  NanAssignPersistent(constructor, tpl->GetFunction());
}

NAN_METHOD(MPQTFile::New) {
  NanScope();

  MPQTFile* obj = new MPQTFile();
  obj->Wrap(args.This());

  NanReturnValue(args.This());
}

Handle<Value> MPQTFile::NewInstance(HANDLE hFile) {
  NanEscapableScope();

  Local<Function> cons = NanNew<Function>(constructor);
  Local<Object> instance = cons->NewInstance();

  MPQTFile* obj = ObjectWrap::Unwrap<MPQTFile>(instance);
  obj->hFile = hFile;

  return NanEscapeScope(instance);
}

NAN_METHOD(MPQTFile::GetFileSize) {
  NanScope();
  MPQTFile* obj = ObjectWrap::Unwrap<MPQTFile>(args.This());

  DWORD filesize = SFileGetFileSize( obj->hFile, NULL );

  NanReturnValue(NanNew<Number>(filesize));
}

NAN_METHOD(MPQTFile::Read) {
  NanScope();
  MPQTFile* obj = ObjectWrap::Unwrap<MPQTFile>(args.This());

  DWORD filesize = SFileGetFileSize( obj->hFile, NULL );
  unsigned char *buffer = new unsigned char[filesize];
  unsigned int actual = 0;

  SFileReadFile( obj->hFile, buffer, (DWORD)filesize, &actual, NULL );

  NanReturnValue(makeBuffer((char *)buffer, actual));
}

NAN_METHOD(MPQTFile::Write) {
  NanScope();
  MPQTFile* obj = ObjectWrap::Unwrap<MPQTFile>(args.This());

  DWORD filesize = SFileGetFileSize( obj->hFile, NULL );
  char *buffer = node::Buffer::Data( args[0]->ToObject() );

  SFileWriteFile( obj->hFile, buffer, (DWORD)filesize, NULL );

  NanReturnValue(args.This());
}

NAN_METHOD(MPQTFile::Finish) {
  NanScope();
  MPQTFile* obj = ObjectWrap::Unwrap<MPQTFile>(args.This());

  SFileFinishFile( obj->hFile );

  NanReturnValue(args.This());
}

NAN_METHOD(MPQTFile::Close) {
  NanScope();
  MPQTFile* obj = ObjectWrap::Unwrap<MPQTFile>(args.This());

  SFileCloseFile( obj->hFile );

  NanReturnValue(NanNew<Boolean>(TRUE));
}
