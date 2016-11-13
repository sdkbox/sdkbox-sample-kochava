
#include "PluginKochavaLuaHelper.h"
#include "PluginKochava/PluginKochava.h"
#include "SDKBoxLuaHelper.h"

static int gAttributionLuaHander = 0;
static int gBeaconLuaHander = 0;

int lua_PluginInMobiLua_PluginKochava_setAttributionCallback(lua_State* tolua_S) {
    int argc = 0;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"sdkbox.PluginKochava",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1) {
#if COCOS2D_DEBUG >= 1
        if (!toluafix_isfunction(tolua_S, 2 , "LUA_FUNCTION",0,&tolua_err)) {
            goto tolua_lerror;
        }
#endif
        gAttributionLuaHander = toluafix_ref_function(tolua_S, 2, 0);
        sdkbox::PluginKochava::setAttributionCallback([](const std::map<std::string, std::string>* attrs) {
            LuaStack* stack = LUAENGINE->getLuaStack();
            std_map_string_string_to_luaval(stack->getLuaState(), *attrs);
            stack->executeFunctionByHandler(gAttributionLuaHander, 1);
        });

        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "sdkbox.PluginKochava::setAttributionCallback",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_PluginInMobiLua_PluginKochava_setAttributionCallback'.",&tolua_err);
#endif
    return 0;
}

int lua_PluginInMobiLua_PluginKochava_setBeaconCallback(lua_State* tolua_S) {
    int argc = 0;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"sdkbox.PluginKochava",0,&tolua_err)) goto tolua_lerror;
#endif
    
    argc = lua_gettop(tolua_S) - 1;
    
    if (argc == 1) {
#if COCOS2D_DEBUG >= 1
        if (!toluafix_isfunction(tolua_S, 2 , "LUA_FUNCTION",0, &tolua_err)) {
            goto tolua_lerror;
        }
#endif
        gBeaconLuaHander = toluafix_ref_function(tolua_S,2,0);

        sdkbox::PluginKochava::setBeaconCallback([](const char* s) {
            LuaStack* stack = LUAENGINE->getLuaStack();
            stack->pushString(s);
            stack->executeFunctionByHandler(gBeaconLuaHander, 1);
        });

        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "sdkbox.PluginKochava::setBeaconCallback",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_PluginInMobiLua_PluginKochava_setBeaconCallback'.",&tolua_err);
#endif
    return 0;
}

int lua_PluginInMobiLua_PluginKochava_retrieveAttribution(lua_State* tolua_S) {
    int argc = 0;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"sdkbox.PluginKochava",0,&tolua_err)) goto tolua_lerror;
#endif
    
    argc = lua_gettop(tolua_S) - 1;
    
    if (argc == 0) {
        auto attr = sdkbox::PluginKochava::retrieveAttribution();
        std_map_string_string_to_luaval(tolua_S, *attr);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "sdkbox.PluginKochava::retrieveAttribution",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_PluginInMobiLua_PluginKochava_retrieveAttribution'.",&tolua_err);
#endif
    return 0;
}

int extern_PluginKochava(lua_State* L) {
    if (NULL == L) {
        return 0;
    }

    lua_pushstring(L, "sdkbox.PluginKochava");
    lua_rawget(L, LUA_REGISTRYINDEX);
    if (lua_istable(L,-1)) {
        tolua_function(L, "setAttributionCallback", lua_PluginInMobiLua_PluginKochava_setAttributionCallback);
        tolua_function(L, "setBeaconCallback", lua_PluginInMobiLua_PluginKochava_setBeaconCallback);
        tolua_function(L, "retrieveAttribution", lua_PluginInMobiLua_PluginKochava_retrieveAttribution);
    }
    lua_pop(L, 1);

    return 1;
}

TOLUA_API int register_all_PluginKochavaLua_helper(lua_State* L) {
    tolua_module(L,"sdkbox",0);
    tolua_beginmodule(L,"sdkbox");

    extern_PluginKochava(L);

    tolua_endmodule(L);
    return 1;
}


