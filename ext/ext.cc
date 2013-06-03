#include <node.h>
#include <node_object_wrap.h>
#include <v8.h>
#include "node_sync.h"

//===========================================================================
// Exporting function
extern "C" void init(Handle<Object> target)
{
  HandleScope scope;
  Sync::Initialize(target);
}