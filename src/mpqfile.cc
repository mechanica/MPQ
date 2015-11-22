#include <node.h>
#include <node_buffer.h>
#include "mpqfile.h"
#include "utils.h"

using v8::Function;
using v8::FunctionTemplate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

Nan::Persistent<Function> MPQFile::constructor;

void MPQFile::Init() {
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);

  tpl->SetClassName(Nan::New("MPQFile").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "getSize", GetSize);
  Nan::SetPrototypeMethod(tpl, "read", Read);
  Nan::SetPrototypeMethod(tpl, "write", Write);
  Nan::SetPrototypeMethod(tpl, "finish", Finish);
  Nan::SetPrototypeMethod(tpl, "close", Close);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
}

Local<Value> MPQFile::NewInstance(HANDLE handle) {
  Local<Function> cons = Nan::New(constructor);
  Local<Object> instance = cons->NewInstance();

  MPQFile* obj = ObjectWrap::Unwrap<MPQFile>(instance);
  obj->_handle = handle;

  return instance;
}

MPQFile::MPQFile()
  :_handle(NULL) {

}

NAN_METHOD(MPQFile::New) {
  MPQFile* obj = new MPQFile();
  obj->Wrap(info.This());
  info.GetReturnValue().Set(info.This());
}

NAN_METHOD(MPQFile::GetSize) {
  MPQFile* obj = ObjectWrap::Unwrap<MPQFile>(info.This());
  DWORD filesize = SFileGetFileSize(obj->_handle, NULL);
  info.GetReturnValue().Set(filesize);
}

NAN_METHOD(MPQFile::Read) {
  MPQFile* obj = ObjectWrap::Unwrap<MPQFile>(info.This());
  DWORD filesize = SFileGetFileSize(obj->_handle, NULL);
  unsigned char *buffer = new unsigned char[filesize];
  unsigned int actual = 0;
  if (SFileReadFile(obj->_handle, buffer, (DWORD)filesize, &actual, NULL) ||
      // Error code will be returned if less than the request # of bytes are read.
      // Treat it as a success.
      GetLastError() == ERROR_HANDLE_EOF) {
    return info.GetReturnValue().Set(
      Nan::NewBuffer((char*)buffer, actual).ToLocalChecked());
  }
}

NAN_METHOD(MPQFile::Write) {
  bool result = false;

  if (info.Length() != 1 || !info[0]->IsObject()) {
    LOG_ERROR("MPQFile::Write - incorrect parameters.");
  } else {
    MPQFile* obj = ObjectWrap::Unwrap<MPQFile>(info.This());
    DWORD filesize = SFileGetFileSize(obj->_handle, NULL);
    char *buffer = node::Buffer::Data(info[0]->ToObject());
    result = SFileWriteFile(obj->_handle, buffer, (DWORD)filesize, 0x0);
  }

  return info.GetReturnValue().Set(result);
}

NAN_METHOD(MPQFile::Finish) {
  MPQFile* obj = ObjectWrap::Unwrap<MPQFile>(info.This());
  bool result = SFileFinishFile(obj->_handle);
  return info.GetReturnValue().Set(result);
}

NAN_METHOD(MPQFile::Close) {
  MPQFile* obj = ObjectWrap::Unwrap<MPQFile>(info.This());
  bool result = SFileCloseFile(obj->_handle);
  return info.GetReturnValue().Set(result);
}
