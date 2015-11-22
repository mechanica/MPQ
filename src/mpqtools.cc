#include <node.h>
#include <nan.h>
#include "mpqarchive.h"
#include "mpqfile.h"
#include "utils.h"

using v8::FunctionTemplate;
using v8::Local;
using v8::String;
using v8::Value;

NAN_METHOD(OpenArchive) {
  if (info.Length() != 1 || !info[0]->IsString()) {
    LOG_ERROR("OpenArchive - incorrect parameters.");
    return;
  }

  HANDLE archiveHandle;
  String::Utf8Value filename(info[0]);

  if (!SFileOpenArchive(*filename, 0, 0x0, &archiveHandle)) {
    return Nan::ThrowError("Failed to open the archive.");
  }

  info.GetReturnValue().Set(MPQArchive::NewInstance(archiveHandle));
}

NAN_METHOD(CreateArchive) {
  if (info.Length() != 1 || !info[0]->IsString()) {
    LOG_ERROR("CreateArchive - incorrect parameters.");
    return;
  }

  HANDLE archiveHandle;
  String::Utf8Value filename(info[0]);

  if (!SFileCreateArchive(*filename, MPQ_CREATE_ARCHIVE_V2, 10, &archiveHandle)) {
    return Nan::ThrowError("Failed to create the archive.");
  }

  info.GetReturnValue().Set(MPQArchive::NewInstance(archiveHandle));
}

NAN_MODULE_INIT(InitAll) {
  MPQArchive::Init();
  MPQFile::Init();

  Nan::Set(target, Nan::New("openArchive").ToLocalChecked(),
    Nan::GetFunction(Nan::New<FunctionTemplate>(OpenArchive)).ToLocalChecked());
  Nan::Set(target, Nan::New("createArchive").ToLocalChecked(),
    Nan::GetFunction(Nan::New<FunctionTemplate>(CreateArchive)).ToLocalChecked());
}

NODE_MODULE(mech_mpq, InitAll)
