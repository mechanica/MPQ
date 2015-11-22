#include "mpqarchive.h"
#include "mpqfile.h"
#include "utils.h"

using v8::Function;
using v8::FunctionTemplate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

Nan::Persistent<Function> MPQArchive::constructor;

void MPQArchive::Init() {
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);

  tpl->SetClassName(Nan::New("MPQArchive").ToLocalChecked());
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

Local<Value> MPQArchive::NewInstance(HANDLE handle) {
  Local<Function> cons = Nan::New(constructor);
  Local<Object> instance = cons->NewInstance();

  MPQArchive* obj = ObjectWrap::Unwrap<MPQArchive>(instance);
  obj->_handle = handle;

  return instance;
}

MPQArchive::MPQArchive()
  :_handle(NULL) {

}

NAN_METHOD(MPQArchive::New) {
  MPQArchive* obj = new MPQArchive();
  obj->Wrap(info.This());
  info.GetReturnValue().Set(info.This());
}

NAN_METHOD(MPQArchive::OpenFile) {
  if (info.Length() != 1 || !info[0]->IsString()) {
    LOG_ERROR("MPQArchive::OpenFile - incorrect parameters.");
  } else {
    MPQArchive* obj = ObjectWrap::Unwrap<MPQArchive>(info.This());
    String::Utf8Value filename(info[0]);

    HANDLE fileHandle;
    if (SFileOpenFileEx(obj->_handle, *filename, SFILE_OPEN_FROM_MPQ, &fileHandle)) {
      return info.GetReturnValue().Set(MPQFile::NewInstance(fileHandle));
    }
  }
}

NAN_METHOD(MPQArchive::HasFile) {
  bool result = false;

  if (info.Length() != 1 || !info[0]->IsString()) {
    LOG_ERROR("MPQArchive::HasFile - incorrect parameters.");
  } else {
    MPQArchive* obj = ObjectWrap::Unwrap<MPQArchive>(info.This());
    String::Utf8Value filename(info[0]);
    result = SFileHasFile(obj->_handle, *filename);
  }

  info.GetReturnValue().Set(result);
}

NAN_METHOD(MPQArchive::CreateFile) {
  if (info.Length() != 2 || !info[0]->IsString() || !info[1]->IsNumber()) {
    LOG_ERROR("MPQArchive::CreateFile - incorrect parameters.");
  } else {
    MPQArchive* obj = ObjectWrap::Unwrap<MPQArchive>(info.This());
    String::Utf8Value filename(info[0]);
    int size = info[1]->NumberValue();

    HANDLE fileHandle;
    if (SFileCreateFile(obj->_handle, *filename, 0, size, 0, 0x0, &fileHandle)) {
      return info.GetReturnValue().Set(MPQFile::NewInstance(fileHandle));
    }
  }
}

NAN_METHOD(MPQArchive::RemoveFile) {
  bool result = false;

  if (info.Length() != 1 || !info[0]->IsString()) {
    LOG_ERROR("MPQArchive::RemoveFile - incorrect parameters.");
  } else {
    MPQArchive* obj = ObjectWrap::Unwrap<MPQArchive>(info.This());
    String::Utf8Value filename(info[0]);
    result = SFileRemoveFile(obj->_handle, *filename, 0);
  }

  return info.GetReturnValue().Set(result);
}

NAN_METHOD(MPQArchive::RenameFile) {
  bool result = false;

  if (info.Length() != 2 || !info[0]->IsString() || !info[1]->IsString()) {
    LOG_ERROR("MPQArchive::RenameFile - incorrect parameters.");
  } else {
    MPQArchive* obj = ObjectWrap::Unwrap<MPQArchive>(info.This());
    String::Utf8Value filename(info[0]);
    String::Utf8Value newFilename(info[1]);
    result = SFileRenameFile(obj->_handle, *filename, *newFilename);
  }

  return info.GetReturnValue().Set(result);
}

NAN_METHOD(MPQArchive::Flush) {
  MPQArchive* obj = ObjectWrap::Unwrap<MPQArchive>(info.This());

  bool result = SFileFlushArchive(obj->_handle);
  return info.GetReturnValue().Set(result);
}

NAN_METHOD(MPQArchive::Close) {
  MPQArchive* obj = ObjectWrap::Unwrap<MPQArchive>(info.This());
  SFileCloseArchive(obj->_handle);
}
