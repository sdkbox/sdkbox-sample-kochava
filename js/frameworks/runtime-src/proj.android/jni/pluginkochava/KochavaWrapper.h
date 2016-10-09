#ifndef __KOCHAVA_WRAPPER_H__
#define __KOCHAVA_WRAPPER_H__

#include "CocosMacros.h"
#include "json98.h"
#include "PluginKochava.h"

NS_COCOS_BEGIN

#define TAG "kochava"
#define VERSION "20160914"
#define IOS_VERSION     "ios: 20160921"
#define ANDROID_VERSION "android: 20160914"

class KochavaWrapper {
public:

    static KochavaWrapper* getInstance();

    virtual void init()=0;
    virtual void shutdown()=0;
    virtual void enableLogging(bool enabled)=0;
    virtual void trackEvent(const char* event, const char* value)=0;
    virtual void spatialEvent(const char* title, float x, float y, float z)=0;
    virtual void setLimitAdTracking(bool limitAdTracking)=0;
    virtual const std::map<std::string, std::string>* retrieveAttribution()=0;
    virtual void sendDeepLink(const char* url, const char* application)=0;
    virtual void setAttributionCallback(PluginKochava::kochavaCallbackDict callback)=0;
    virtual void setBeaconCallback(PluginKochava::kochavaCallbackString callback)=0;
    virtual void attributionCallback(const std::map<std::string, std::string>* payload)=0;
    virtual void beaconCallback(const char* payload)=0;
    virtual void setAttribution(const std::map<std::string, std::string>& attribution)=0;
    virtual void addAttribution(const std::string& key, const std::string& val)=0;
    virtual const std::map<std::string, std::string>* getAttribution()=0;
};

class KochavaWrapperDisabled : public KochavaWrapper {
public:
    void init() {}
    void shutdown() {}
    void enableLogging(bool enabled) {}
    void trackEvent(const char* event, const char* value) {}
    void spatialEvent(const char* title, float x, float y, float z) {}
    void setLimitAdTracking(bool limitAdTracking) {}
    const std::map<std::string, std::string>* retrieveAttribution() { return NULL; }
    void sendDeepLink(const char* url, const char* application) {}
    void setAttributionCallback(PluginKochava::kochavaCallbackDict callback) {}
    void setBeaconCallback(PluginKochava::kochavaCallbackString callback) {}
    void attributionCallback(const std::map<std::string, std::string>* payload) {}
    void beaconCallback(const char* payload) {}
    void setAttribution(const std::map<std::string, std::string>& attribution) {}
    void addAttribution(const std::string& key, const std::string& val) {}
    inline const std::map<std::string, std::string>* getAttribution() { return NULL; }
};


class KochavaWrapperEnabled : public KochavaWrapper {
public:
    void init();
    void shutdown();
    void enableLogging(bool enabled);
    void trackEvent(const char* event, const char* value);
    void spatialEvent(const char* title, float x, float y, float z);
    void setLimitAdTracking(bool limitAdTracking);
    const std::map<std::string, std::string>* retrieveAttribution();
    void sendDeepLink(const char* url, const char* application);
    void setAttributionCallback(PluginKochava::kochavaCallbackDict callback);
    void setBeaconCallback(PluginKochava::kochavaCallbackString callback);

    void attributionCallback(const std::map<std::string, std::string>* payload);
    void beaconCallback(const char* payload);
    void setAttribution(const std::map<std::string, std::string>& attribution);
    void addAttribution(const std::string& key, const std::string& val);
    inline const std::map<std::string, std::string>* getAttribution() { return &_attribution; }
    KochavaWrapperEnabled();

protected:
    bool nativeInit(Json& config);
    const char* nativeSDKVersion();


private:
    PluginKochava::kochavaCallbackDict _attributionCallback;
    PluginKochava::kochavaCallbackString _beaconCallback;
    std::map<std::string, std::string> _attribution;
};

#endif

NS_COCOS_END
