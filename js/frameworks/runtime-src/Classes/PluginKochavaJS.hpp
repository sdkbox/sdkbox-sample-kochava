#ifndef __PluginKochavaJS_h__
#define __PluginKochavaJS_h__

#include "jsapi.h"
#include "jsfriendapi.h"


#if MOZJS_MAJOR_VERSION >= 33
void js_register_PluginKochavaJS_PluginKochava(JSContext *cx, JS::HandleObject global);
void register_all_PluginKochavaJS(JSContext* cx, JS::HandleObject obj);
#else
void js_register_PluginKochavaJS_PluginKochava(JSContext *cx, JSObject* global);
void register_all_PluginKochavaJS(JSContext* cx, JSObject* obj);
#endif
#endif

