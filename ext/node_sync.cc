#include <stdarg.h>
#include <cstdlib>
#include <cstring>
#include <string.h>
#include <stdlib.h>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif

#include <v8.h>

// this and the above block must be around the v8.h header otherwise
// v8 is not happy
#ifdef __clang__
#pragma clang diagnostic pop
#endif

#include <node.h>
#include <node_version.h>
#include <node_buffer.h>

#include <cmath>
#include <iostream>
#include <limits>
#include <vector>

#ifdef __sun
  #include <alloca.h>
#endif

#include <pthread.h>

#include "node_sync.h"
#include <uv.h>

using namespace v8;
using namespace node;

Persistent<FunctionTemplate> Sync::constructor_template;

static Handle<Value> VException(const char *msg) {
  HandleScope scope;
  return ThrowException(Exception::Error(String::New(msg)));
}

Sync::Sync() : ObjectWrap() {
}

void Sync::Initialize(v8::Handle<v8::Object> target) {
  // Grab the scope of the call from Node
  HandleScope scope;
  // Define a new function template
  Local<FunctionTemplate> t = FunctionTemplate::New(New);
  constructor_template = Persistent<FunctionTemplate>::New(t);
  constructor_template->InstanceTemplate()->SetInternalFieldCount(1);
  constructor_template->SetClassName(String::NewSymbol("Sync"));

  // Instance methods
  NODE_SET_PROTOTYPE_METHOD(constructor_template, "execute", Execute);

  // Set the class name
  target->ForceSet(String::NewSymbol("Sync"), constructor_template->GetFunction());
}

// Create a new instance of BSON and passing it the existing context
Handle<Value> Sync::New(const Arguments &args) {
  HandleScope scope;

  Sync *sync = new Sync();
  sync->Wrap(args.This());
  return args.This();
}

Handle<Value> Sync::Execute(const Arguments &args) {
  HandleScope scope;

  if(args.Length() < 2) return VException("Function and context must be first two parameters");
  if(!args[0]->IsFunction()) return VException("First parameter must be a Function");
  if(!args[1]->IsObject()) return VException("Second parameter must be a context object");

  // Our original function we are wrapping
  v8::Persistent<v8::Function> function;
  function = v8::Persistent<v8::Function>::New(Handle<Function>::Cast(args[0]));

  // The current global context
  Handle<v8::Object> global = v8::Context::GetCurrent()->Global();  
  // Compile the callback we will use
  Handle<String> source = String::New("var return_value = null; function my_callback(err, result) { return_value = {err: err, result: result }; }");
  Handle<Script> script = Script::Compile(source);
  script->Run();
  
  // Used to reference the callback function
  Handle<v8::Value> value = global->Get(String::New("my_callback"));
  Handle<Function> func = v8::Handle<v8::Function>::Cast(value);

  // Handles the js result from the callback
  Handle<Value> js_result;

  // Total number of additional function parameters
  const int length = args.Length() - 2;

  // Allocate the length of the argument
  Handle<Value> *_args = new Handle<Value>[length];

  // Map the values (from 3rd argument one it's the parameters passed to the method)
  for(int i = 0; i < length; i++) {
    _args[i] = args[2 + i];
  }

  // Last parameter is the callback
  _args[length] = func;

  // Call function
  function->Call(args[1]->ToObject(), length + 1, _args);
  // Retrieve the return value
  js_result = global->Get(String::New("return_value"));

  // While we have not result let event loop run a step at a time
  while(js_result->IsNull()) {
    // Run event loop for a tick
    uv_run_once(uv_default_loop());    
    // Check for return value
    js_result = global->Get(String::New("return_value"));
  }

  // Close the scope and return the final result
  return scope.Close(js_result);
}

NODE_MODULE(node_sync, Sync::Initialize);
