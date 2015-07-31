#include <node.h>
#include <node_buffer.h>
#include "mpqtfile.h"


using namespace v8;

static Local<Object> makeBuffer(char* data, size_t size) {
  HandleScope scope;

  Local<node::Buffer> slowBuffer = node::Buffer::New(data, size);
  Local<Object> global = Context::GetCurrent()->Global();
  Local<Value> bv = global->Get(String::NewSymbol("Buffer"));
  assert(bv->IsFunction());
  Local<Function> b = Local<Function>::Cast(bv);
  Handle<Value> argv[3] = { slowBuffer->handle_, Integer::New(size), Integer::New(0) };
  Local<Object> fastBuffer = b->NewInstance(3, argv);

  return scope.Close(fastBuffer);
}

MPQTFile::MPQTFile() {};
MPQTFile::~MPQTFile() {};

Persistent<Function> MPQTFile::constructor;

void MPQTFile::Init() {
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("MPQTFile"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  tpl->PrototypeTemplate()->Set(String::NewSymbol("getFileSize"), FunctionTemplate::New(GetFileSize)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("readFile"), FunctionTemplate::New(Read)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("writeFile"), FunctionTemplate::New(Write)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("finishFile"), FunctionTemplate::New(Finish)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("closeFile"), FunctionTemplate::New(Close)->GetFunction());

  constructor = Persistent<Function>::New(tpl->GetFunction());
}

Handle<Value> MPQTFile::New(const Arguments& args) {
  HandleScope scope;

  MPQTFile* obj = new MPQTFile();
  obj->Wrap(args.This());

  return args.This();
}

Handle<Value> MPQTFile::NewInstance(HANDLE hFile) {
  HandleScope scope;

  Local<Object> instance = constructor->NewInstance();

  MPQTFile* obj = ObjectWrap::Unwrap<MPQTFile>(instance);
  obj->hFile = hFile;

  return scope.Close(instance);
}

Handle<Value> MPQTFile::GetFileSize(const Arguments& args) {
  HandleScope scope;
  MPQTFile* obj = ObjectWrap::Unwrap<MPQTFile>(args.This());

  DWORD filesize = SFileGetFileSize( obj->hFile, NULL );

  return scope.Close(Number::New(filesize));
}

Handle<Value> MPQTFile::Read(const Arguments& args) {
  HandleScope scope;
  MPQTFile* obj = ObjectWrap::Unwrap<MPQTFile>(args.This());

  DWORD filesize = SFileGetFileSize( obj->hFile, NULL );
  unsigned char *buffer = new unsigned char[filesize];
  unsigned int actual = 0;

  SFileReadFile( obj->hFile, buffer, (DWORD)filesize, &actual, NULL );

  return scope.Close( makeBuffer( (char *)buffer, actual ) );
}

Handle<Value> MPQTFile::Write(const Arguments& args) {
  HandleScope scope;
  MPQTFile* obj = ObjectWrap::Unwrap<MPQTFile>(args.This());

  DWORD filesize = SFileGetFileSize( obj->hFile, NULL );
  char *buffer = node::Buffer::Data( args[0]->ToObject() );

  SFileWriteFile( obj->hFile, buffer, (DWORD)filesize, NULL );

  return scope.Close( args.This() );
}

Handle<Value> MPQTFile::Finish(const Arguments& args) {
  HandleScope scope;
  MPQTFile* obj = ObjectWrap::Unwrap<MPQTFile>(args.This());

  SFileFinishFile( obj->hFile );

  return scope.Close( args.This() );
}

Handle<Value> MPQTFile::Close(const Arguments& args) {
  HandleScope scope;
  MPQTFile* obj = ObjectWrap::Unwrap<MPQTFile>(args.This());

  SFileCloseFile( obj->hFile );

  return scope.Close( Boolean::New(TRUE) );
}
