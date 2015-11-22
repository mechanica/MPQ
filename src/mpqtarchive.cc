#include "mpqtarchive.h"
#include "mpqtfile.h"
#include "utils.h"

using v8::Function;
using v8::FunctionTemplate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

Nan::Persistent<Function> MPQTArchive::constructor;

void MPQTArchive::Init() {
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);

  tpl->SetClassName(Nan::New("MPQTArchive").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "openFile", OpenFile);
  Nan::SetPrototypeMethod(tpl, "hasFile", HasFile);
  Nan::SetPrototypeMethod(tpl, "createFile", CreateFile);
  Nan::SetPrototypeMethod(tpl, "removeFile", RemoveFile);
  Nan::SetPrototypeMethod(tpl, "renameFile", RenameFile);
  Nan::SetPrototypeMethod(tpl, "flush", Flush);
  Nan::SetPrototypeMethod(tpl, "close", Close);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
}

Local<Value> MPQTArchive::NewInstance(HANDLE handle) {
  Local<Function> cons = Nan::New(constructor);
  Local<Object> instance = cons->NewInstance();

  MPQTArchive* obj = ObjectWrap::Unwrap<MPQTArchive>(instance);
  obj->_handle = handle;

  return instance;
}

MPQTArchive::MPQTArchive()
  :_handle(NULL) {

}

NAN_METHOD(MPQTArchive::New) {
  MPQTArchive* obj = new MPQTArchive();
  obj->Wrap(info.This());
  info.GetReturnValue().Set(info.This());
}

NAN_METHOD(MPQTArchive::OpenFile) {
  if (info.Length() != 1 || !info[0]->IsString()) {
    LOG_ERROR("MPQTArchive::OpenFile - incorrect parameters.");
  } else {
    MPQTArchive* obj = ObjectWrap::Unwrap<MPQTArchive>(info.This());
    String::Utf8Value filename(info[0]);

    HANDLE fileHandle;
    if (SFileOpenFileEx(obj->_handle, *filename, SFILE_OPEN_FROM_MPQ, &fileHandle)) {
      return info.GetReturnValue().Set(MPQTFile::NewInstance(fileHandle));
    }
  }
}

NAN_METHOD(MPQTArchive::HasFile) {
  bool result = false;

  if (info.Length() != 1 || !info[0]->IsString()) {
    LOG_ERROR("MPQTArchive::HasFile - incorrect parameters.");
  } else {
    MPQTArchive* obj = ObjectWrap::Unwrap<MPQTArchive>(info.This());
    String::Utf8Value filename(info[0]);
    result = SFileHasFile(obj->_handle, *filename);
  }

  info.GetReturnValue().Set(result);
}

NAN_METHOD(MPQTArchive::CreateFile) {
  if (info.Length() != 2 || !info[0]->IsString() || !info[1]->IsNumber()) {
    LOG_ERROR("MPQTArchive::CreateFile - incorrect parameters.");
  } else {
    MPQTArchive* obj = ObjectWrap::Unwrap<MPQTArchive>(info.This());
    String::Utf8Value filename(info[0]);
    int size = info[1]->NumberValue();

    HANDLE fileHandle;
    if (SFileCreateFile(obj->_handle, *filename, 0, size, 0, 0x0, &fileHandle)) {
      return info.GetReturnValue().Set(MPQTFile::NewInstance(fileHandle));
    }
  }
}

NAN_METHOD(MPQTArchive::RemoveFile) {
  bool result = false;

  if (info.Length() != 1 || !info[0]->IsString()) {
    LOG_ERROR("MPQTArchive::RemoveFile - incorrect parameters.");
  } else {
    MPQTArchive* obj = ObjectWrap::Unwrap<MPQTArchive>(info.This());
    String::Utf8Value filename(info[0]);
    result = SFileRemoveFile(obj->_handle, *filename, 0);
  }

  return info.GetReturnValue().Set(result);
}

NAN_METHOD(MPQTArchive::RenameFile) {
  bool result = false;

  if (info.Length() != 2 || !info[0]->IsString() || !info[1]->IsString()) {
    LOG_ERROR("MPQTArchive::RenameFile - incorrect parameters.");
  } else {
    MPQTArchive* obj = ObjectWrap::Unwrap<MPQTArchive>(info.This());
    String::Utf8Value filename(info[0]);
    String::Utf8Value newFilename(info[1]);
    result = SFileRenameFile(obj->_handle, *filename, *newFilename);
  }

  return info.GetReturnValue().Set(result);
}

NAN_METHOD(MPQTArchive::Flush) {
  MPQTArchive* obj = ObjectWrap::Unwrap<MPQTArchive>(info.This());

  bool result = SFileFlushArchive(obj->_handle);
  return info.GetReturnValue().Set(result);
}

NAN_METHOD(MPQTArchive::Close) {
  MPQTArchive* obj = ObjectWrap::Unwrap<MPQTArchive>(info.This());
  SFileCloseArchive(obj->_handle);
}
