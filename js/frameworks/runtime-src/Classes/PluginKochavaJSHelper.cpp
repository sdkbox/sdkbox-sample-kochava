
#include "PluginKochavaJSHelper.h"
#include "SDKBoxJSHelper.h"
#include "PluginKochava/PluginKochava.h"

#if (COCOS2D_VERSION < 0x00030000)
#define Ref CCObject
#define Director CCDirector
#define getInstance sharedDirector
#define schedule scheduleSelector
#endif

USING_NS_CC;

static JSContext* s_cx = nullptr;

class JsKochavaCallbackObj : public Ref {
public:

    static JsKochavaCallbackObj* create(const std::string& name, JSObject *handler) {
        JsKochavaCallbackObj *obj = new JsKochavaCallbackObj;
        obj->_eventName = name;
        obj->_jsHandler = handler;
        obj->autorelease();
        return obj;
    }

	void setBeaconData(const std::string& data) {
		_beaconData = data;
	}

	void setAttributionData(const std::map<std::string, std::string>& data) {
		_attributionData = data;
	}

    void start() {
        Director::getInstance()->getScheduler()->schedule(schedule_selector(JsKochavaCallbackObj::callback), this, 0.1, 0, 0.0f, false);
    }

    void callback(float dt) {
        if (!s_cx)
            return;

        JSContext* cx = s_cx;
        const char* func_name = _eventName.data();

        JS::RootedObject obj(cx, _jsHandler);
        JSAutoCompartment ac(cx, obj);

#if defined(MOZJS_MAJOR_VERSION)
#if MOZJS_MAJOR_VERSION >= 33
        bool hasAction;
        JS::RootedValue retval(cx);
        JS::RootedValue func_handle(cx);
#else
        bool hasAction;
        jsval retval;
        JS::RootedValue func_handle(cx);
#endif
#elif defined(JS_VERSION)
        JSBool hasAction;
        jsval retval;
        jsval func_handle;
#endif

        jsval dataVal[2];
        int datalen = 0;

        if (0 == _eventName.compare("attributionCallback"))
        {
            dataVal[0] = std_map_string_string_to_jsval(cx, _attributionData);
            datalen = 1;
        }
        else if (0 == _eventName.compare("beaconCallback"))
        {
            dataVal[0] = std_string_to_jsval(cx, _beaconData);
            datalen = 1;
        }
        else
        {
            return;
        }

        if (JS_HasProperty(cx, obj, func_name, &hasAction) && hasAction)
        {
            if (!JS_GetProperty(cx, obj, func_name, &func_handle))
                return;
            if (func_handle == JSVAL_VOID)
                return;

#if MOZJS_MAJOR_VERSION >= 31
            JS_CallFunctionName(cx, obj, func_name, JS::HandleValueArray::fromMarkedLocation(datalen, dataVal), &retval);
#else
            JS_CallFunctionName(cx, obj, func_name, datalen, dataVal, &retval);
#endif
        }
        release();
    }

protected:

    JsKochavaCallbackObj()
        : _eventName("")
        , _jsHandler(nullptr) {
        retain();
    }
    JSObject*   _jsHandler;
    std::string _eventName;
    std::string _beaconData;
    std::map<std::string, std::string> _attributionData;
};

static JSObject* _JSKochavaAttributionDelegate = 0;
static void __kochavaAttributionCallback(const std::map<std::string, std::string>* attributionData) {
	if (_JSKochavaAttributionDelegate && attributionData) {
        JsKochavaCallbackObj* obj = JsKochavaCallbackObj::create("attributionCallback", _JSKochavaAttributionDelegate);
        obj->setAttributionData(*attributionData);
        obj->start();
	}
}

static JSObject* _JSKochavaBeaconDelegate = 0;
static void __kochavaBeaconCallback(const char* beaconData) {
	if (_JSKochavaBeaconDelegate && beaconData) {
        JsKochavaCallbackObj* obj = JsKochavaCallbackObj::create("beaconCallback", _JSKochavaBeaconDelegate);
        obj->setBeaconData(std::string(beaconData));
        obj->start();
	}
}

#if defined(MOZJS_MAJOR_VERSION)
bool js_PluginKochavaJS_PluginKochava_setAttributionCallback(JSContext *cx, uint32_t argc, jsval *vp)
#elif defined(JS_VERSION)
JSBool js_PluginKochavaJS_PluginKochava_setAttributionCallback(JSContext *cx, uint32_t argc, jsval *vp)
#endif
{
    s_cx = cx;
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;

    if (argc == 1)
    {
        if (!args.get(0).isObject())
        {
            ok = false;
        }
        JSObject *tmpObj = args.get(0).toObjectOrNull();

        JSB_PRECONDITION2(ok, cx, false, "js_PluginKochavaJS_setAttributionCallback : Error processing arguments");

        _JSKochavaAttributionDelegate = tmpObj;
        sdkbox::PluginKochava::setAttributionCallback(__kochavaAttributionCallback);

        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_PluginKochavaJS_setAttributionCallback : wrong number of arguments");
    return false;
}

#if defined(MOZJS_MAJOR_VERSION)
bool js_PluginKochavaJS_PluginKochava_setBeaconCallback(JSContext *cx, uint32_t argc, jsval *vp)
#elif defined(JS_VERSION)
JSBool js_PluginKochavaJS_PluginKochava_setBeaconCallback(JSContext *cx, uint32_t argc, jsval *vp)
#endif
{
    s_cx = cx;
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;

    if (argc == 1)
    {
        if (!args.get(0).isObject())
        {
            ok = false;
        }
        JSObject *tmpObj = args.get(0).toObjectOrNull();

        JSB_PRECONDITION2(ok, cx, false, "js_PluginKochavaJS_setBeaconCallback : Error processing arguments");

        _JSKochavaBeaconDelegate = tmpObj;
        sdkbox::PluginKochava::setBeaconCallback(__kochavaBeaconCallback);

        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_PluginKochavaJS_setBeaconCallback : wrong number of arguments");
    return false;
}

#if defined(MOZJS_MAJOR_VERSION)
bool js_PluginKochavaJS_PluginKochava_retrieveAttribution(JSContext *cx, uint32_t argc, jsval *vp)
#elif defined(JS_VERSION)
JSBool js_PluginKochavaJS_PluginKochava_retrieveAttribution(JSContext *cx, uint32_t argc, jsval *vp)
#endif
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {
        const std::map<std::string, std::string>* ret = sdkbox::PluginKochava::retrieveAttribution();
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                jsret = sdkbox::std_map_string_string_to_jsval(cx, *ret);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);

        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_PluginKochavaJS_PluginKochava_retrieveAttribution : wrong number of arguments");
    return false;
}

#define KOCHAVA_JS_FUNCTIONS \
JS_DefineFunction(cx, pluginObj, "retrieveAttribution", js_PluginKochavaJS_PluginKochava_retrieveAttribution, 0, JSPROP_READONLY | JSPROP_PERMANENT);

#if defined(MOZJS_MAJOR_VERSION)
#if MOZJS_MAJOR_VERSION >= 33
void register_all_PluginKochavaJS_helper(JSContext* cx, JS::HandleObject global) {
    // Get the ns
    JS::RootedObject pluginObj(cx);
    sdkbox::getJsObjOrCreat(cx, global, "sdkbox.PluginKochava", &pluginObj);

    JS_DefineFunction(cx, pluginObj, "setAttributionCallback", js_PluginKochavaJS_PluginKochava_setAttributionCallback, 1, JSPROP_READONLY | JSPROP_PERMANENT);
    JS_DefineFunction(cx, pluginObj, "setBeaconCallback", js_PluginKochavaJS_PluginKochava_setBeaconCallback, 1, JSPROP_READONLY | JSPROP_PERMANENT);

    KOCHAVA_JS_FUNCTIONS
}
#else
void register_all_PluginKochavaJS_helper(JSContext* cx, JSObject* obj) {
    JS::RootedObject pluginObj(cx);
    sdkbox::getJsObjOrCreat(cx, JS::RootedObject(cx, global), "sdkbox.PluginKochava", &pluginObj);

    JS_DefineFunction(cx, pluginObj, "setAttributionCallback", js_PluginKochavaJS_setAttributionCallback, 1, JSPROP_READONLY | JSPROP_PERMANENT);
    JS_DefineFunction(cx, pluginObj, "setBeaconCallback", js_PluginKochavaJS_setBeaconCallback, 1, JSPROP_READONLY | JSPROP_PERMANENT);

    KOCHAVA_JS_FUNCTIONS
}
#endif
#elif defined(JS_VERSION)
void register_all_PluginKochavaJS_helper(JSContext* cx, JSObject* global) {
    jsval pluginVal;
    JSObject* pluginObj;
    pluginVal = sdkbox::getJsObjOrCreat(cx, global, "sdkbox.PluginKochava", &pluginObj);

    JS_DefineFunction(cx, pluginObj, "setAttributionCallback", js_PluginKochavaJS_setAttributionCallback, 1, JSPROP_READONLY | JSPROP_PERMANENT);
    JS_DefineFunction(cx, pluginObj, "setBeaconCallback", js_PluginKochavaJS_setBeaconCallback, 1, JSPROP_READONLY | JSPROP_PERMANENT);

    KOCHAVA_JS_FUNCTIONS
}
#endif

