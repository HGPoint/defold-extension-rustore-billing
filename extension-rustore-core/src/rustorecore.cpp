#define EXTENSION_NAME RuStoreCore
#define LIB_NAME "RuStoreCore"
#define MODULE_NAME "rustorecore"

#if defined(DM_PLATFORM_ANDROID)

#include <dmsdk/sdk.h>
#include <dmsdk/dlib/android.h>
#include "RuStoreChannelListener.h"
#include "ChannelCallbackManager.h"
#include "AndroidJavaObject.h"

using namespace RuStoreSDK;

static void GatJavaCoreInstance(JNIEnv* env, AndroidJavaObject* instance)
{
    jclass cls = dmAndroid::LoadClass(env, "ru.rustore.defold.core.RuStoreCore");
    jfieldID instanceField = env->GetStaticFieldID(cls, "INSTANCE", "Lru/rustore/defold/core/RuStoreCore;");
    jobject obj = env->GetStaticObjectField(cls, instanceField);

    instance->cls = cls;
    instance->obj = obj;
}

static void InitDefoldPlayer()
{
    dmAndroid::ThreadAttacher thread;
    JNIEnv* env = thread.GetEnv();

    jclass jplayerCls = dmAndroid::LoadClass(env, "ru.rustore.defold.core.wrappers.DefoldPlayerWrapper");
    jobject jplayerObj = env->NewObject(jplayerCls, env->GetMethodID(jplayerCls, "<init>", "()V"));

    jclass jproviderCls = dmAndroid::LoadClass(env, "ru.rustore.defold.core.PlayerProvider");
    jfieldID instanceField = env->GetStaticFieldID(jproviderCls, "INSTANCE", "Lru/rustore/defold/core/PlayerProvider;");
    jobject playerProviderInstance = env->GetStaticObjectField(jproviderCls, instanceField);

    jmethodID method = env->GetMethodID(jproviderCls, "setExternalProvider", "(Lru/rustore/defold/core/IPlayerProvider;)V");
    env->CallVoidMethod(playerProviderInstance, method, jplayerObj);
    
    thread.Detach();
}

static void InitRuStoreCallbacks()
{
    dmAndroid::ThreadAttacher thread;
    JNIEnv* env = thread.GetEnv();

    jobject wrapper = RuStoreChannelListener::Instance()->GetJWrapper();
    
    AndroidJavaObject instance;
    GatJavaCoreInstance(env, &instance);
    jmethodID method = env->GetMethodID(instance.cls, "setChannelListener", "(Lru/rustore/defold/core/callbacks/IRuStoreChannelListener;)V");
    env->CallVoidMethod(instance.obj, method, wrapper);

    thread.Detach();
}

static int Connect(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);

    const char* channel = (char*)luaL_checkstring(L, 1);
    dmScript::LuaCallbackInfo* callback = dmScript::CreateCallback(L, 2);

    auto channelCallbackItem = std::make_shared<ChannelCallbackItem>(channel, callback);
    ChannelCallbackManager::Instance()->AddChannelCallback(channelCallbackItem);

    return 0;
}

int ConnectCallback(const char* channel, dmScript::LuaCallbackInfo* callback)
{
    dmLogInfo("ConnectCallback %s", channel);

    auto channelCallbackItem = std::make_shared<ChannelCallbackItem>(channel, callback);
    ChannelCallbackManager::Instance()->AddChannelCallback(channelCallbackItem);

    return 0;
}

static int ShowToast(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);

    dmAndroid::ThreadAttacher thread;
    JNIEnv* env = thread.GetEnv();

    const char* msg = (char*)luaL_checkstring(L, 1);
    jstring jmsg = env->NewStringUTF(msg);
    
    AndroidJavaObject instance;
    GatJavaCoreInstance(env, &instance);
    jmethodID method = env->GetMethodID(instance.cls, "showToast", "(Landroid/app/Activity;Ljava/lang/String;)V");
    env->CallVoidMethod(instance.obj, method, dmGraphics::GetNativeAndroidActivity(), jmsg);

    env->DeleteLocalRef(jmsg);

    thread.Detach();

    return 0;
}

static int LogVerbose(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);

    dmAndroid::ThreadAttacher thread;
    JNIEnv* env = thread.GetEnv();

    const char* tag = (char*)luaL_checkstring(L, 1);
    const char* msg = (char*)luaL_checkstring(L, 2);

    jstring jtag = env->NewStringUTF(tag);
    jstring jmsg = env->NewStringUTF(msg);
    
    AndroidJavaObject instance;
    GatJavaCoreInstance(env, &instance);
    jmethodID method = env->GetMethodID(instance.cls, "logVerbose", "(Ljava/lang/String;Ljava/lang/String;)V");
    env->CallVoidMethod(instance.obj, method, jtag, jmsg);

    env->DeleteLocalRef(jtag);
    env->DeleteLocalRef(jmsg);

    thread.Detach();

    return 0;
}

static int LogDebug(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);

    dmAndroid::ThreadAttacher thread;
    JNIEnv* env = thread.GetEnv();

    const char* tag = (char*)luaL_checkstring(L, 1);
    const char* msg = (char*)luaL_checkstring(L, 2);

    jstring jtag = env->NewStringUTF(tag);
    jstring jmsg = env->NewStringUTF(msg);
    
    AndroidJavaObject instance;
    GatJavaCoreInstance(env, &instance);
    jmethodID method = env->GetMethodID(instance.cls, "logDebug", "(Ljava/lang/String;Ljava/lang/String;)V");
    env->CallVoidMethod(instance.obj, method, jtag, jmsg);

    env->DeleteLocalRef(jtag);
    env->DeleteLocalRef(jmsg);

    thread.Detach();

    return 0;
}

static int LogInfo(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);

    dmAndroid::ThreadAttacher thread;
    JNIEnv* env = thread.GetEnv();

    const char* tag = (char*)luaL_checkstring(L, 1);
    const char* msg = (char*)luaL_checkstring(L, 2);

    jstring jtag = env->NewStringUTF(tag);
    jstring jmsg = env->NewStringUTF(msg);
    
    AndroidJavaObject instance;
    GatJavaCoreInstance(env, &instance);
    jmethodID method = env->GetMethodID(instance.cls, "logInfo", "(Ljava/lang/String;Ljava/lang/String;)V");
    env->CallVoidMethod(instance.obj, method, jtag, jmsg);

    env->DeleteLocalRef(jtag);
    env->DeleteLocalRef(jmsg);

    thread.Detach();

    return 0;
}

static int LogWarning(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);

    dmAndroid::ThreadAttacher thread;
    JNIEnv* env = thread.GetEnv();

    const char* tag = (char*)luaL_checkstring(L, 1);
    const char* msg = (char*)luaL_checkstring(L, 2);

    jstring jtag = env->NewStringUTF(tag);
    jstring jmsg = env->NewStringUTF(msg);
    
    AndroidJavaObject instance;
    GatJavaCoreInstance(env, &instance);
    jmethodID method = env->GetMethodID(instance.cls, "logWarning", "(Ljava/lang/String;Ljava/lang/String;)V");
    env->CallVoidMethod(instance.obj, method, jtag, jmsg);

    env->DeleteLocalRef(jtag);
    env->DeleteLocalRef(jmsg);
    
    thread.Detach();
    
    return 0;
}

static int LogError(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);

    dmAndroid::ThreadAttacher thread;
    JNIEnv* env = thread.GetEnv();

    const char* tag = (char*)luaL_checkstring(L, 1);
    const char* msg = (char*)luaL_checkstring(L, 2);

    jstring jtag = env->NewStringUTF(tag);
    jstring jmsg = env->NewStringUTF(msg);
    
    AndroidJavaObject instance;
    GatJavaCoreInstance(env, &instance);
    jmethodID method = env->GetMethodID(instance.cls, "logError", "(Ljava/lang/String;Ljava/lang/String;)V");
    env->CallVoidMethod(instance.obj, method, jtag, jmsg);

    env->DeleteLocalRef(jtag);
    env->DeleteLocalRef(jmsg);

    thread.Detach();

    return 0;
}

static int CopyToClipboard(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);

    dmAndroid::ThreadAttacher thread;
    JNIEnv* env = thread.GetEnv();

    const char* text = (char*)luaL_checkstring(L, 1);
    jstring jtext = env->NewStringUTF(text);
    
    AndroidJavaObject instance;
    GatJavaCoreInstance(env, &instance);
    jmethodID method = env->GetMethodID(instance.cls, "copyToClipboard", "(Landroid/app/Activity;Ljava/lang/String;)V");
    env->CallVoidMethod(instance.obj, method, dmGraphics::GetNativeAndroidActivity(), jtext);

    env->DeleteLocalRef(jtext);

    thread.Detach();

    return 0;
}

static int GetFromClipboard(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);

    dmAndroid::ThreadAttacher thread;
    JNIEnv* env = thread.GetEnv();

    AndroidJavaObject instance;
    GatJavaCoreInstance(env, &instance);
    jmethodID method = env->GetMethodID(instance.cls, "getFromClipboard", "(Landroid/app/Activity;)Ljava/lang/String;");
    jstring jtext = (jstring)env->CallObjectMethod(instance.obj, method, dmGraphics::GetNativeAndroidActivity());

    const char* ctext = env->GetStringUTFChars(jtext, nullptr);
    lua_pushstring(L, ctext);
    env->ReleaseStringUTFChars(jtext, ctext);
    
    thread.Detach();
    
    return 1;
}

static int GetStringResources(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);

    dmAndroid::ThreadAttacher thread;
    JNIEnv* env = thread.GetEnv();

    const char* name = (char*)luaL_checkstring(L, 1);
    jstring jname = env->NewStringUTF(name);

    AndroidJavaObject instance;
    GatJavaCoreInstance(env, &instance);
    jmethodID method = env->GetMethodID(instance.cls, "getStringResources", "(Landroid/app/Activity;Ljava/lang/String;)Ljava/lang/String;");
    jstring jtext = (jstring)env->CallObjectMethod(instance.obj, method, dmGraphics::GetNativeAndroidActivity(), jname);

    const char* ctext = env->GetStringUTFChars(jtext, nullptr);
    lua_pushstring(L, ctext);
    env->ReleaseStringUTFChars(jtext, ctext);
    
    env->DeleteLocalRef(jname);

    thread.Detach();
    
    return 1;
}

static int GetStringSharedPreferences(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);

    dmAndroid::ThreadAttacher thread;
    JNIEnv* env = thread.GetEnv();

    const char* storageName = (char*)luaL_checkstring(L, 1);
    jstring jstorageName = env->NewStringUTF(storageName);

    const char* key = (char*)luaL_checkstring(L, 2);
    jstring jkey = env->NewStringUTF(key);

    const char* defaultValue = (char*)luaL_checkstring(L, 3);
    jstring jdefaultValue = env->NewStringUTF(defaultValue);

    AndroidJavaObject instance;
    GatJavaCoreInstance(env, &instance);
    jmethodID method = env->GetMethodID(instance.cls, "getStringSharedPreferences", "(Landroid/app/Activity;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
    jstring jtext = (jstring)env->CallObjectMethod(instance.obj, method, dmGraphics::GetNativeAndroidActivity(), jstorageName, jkey, jdefaultValue);

    const char* ctext = env->GetStringUTFChars(jtext, nullptr);
    lua_pushstring(L, ctext);
    env->ReleaseStringUTFChars(jtext, ctext);

    env->DeleteLocalRef(jstorageName);
    env->DeleteLocalRef(jkey);
    env->DeleteLocalRef(jdefaultValue);

    thread.Detach();

    return 1;
}



static int SetStringSharedPreferences(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);

    dmAndroid::ThreadAttacher thread;
    JNIEnv* env = thread.GetEnv();

    const char* storageName = (char*)luaL_checkstring(L, 1);
    jstring jstorageName = env->NewStringUTF(storageName);

    const char* key = (char*)luaL_checkstring(L, 2);
    jstring jkey = env->NewStringUTF(key);

    const char* defaultValue = (char*)luaL_checkstring(L, 3);
    jstring jdefaultValue = env->NewStringUTF(defaultValue);

    AndroidJavaObject instance;
    GatJavaCoreInstance(env, &instance);
    jmethodID method = env->GetMethodID(instance.cls, "setStringSharedPreferences", "(Landroid/app/Activity;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    env->CallVoidMethod(instance.obj, method, dmGraphics::GetNativeAndroidActivity(), jstorageName, jkey, jdefaultValue);

    env->DeleteLocalRef(jstorageName);
    env->DeleteLocalRef(jkey);
    env->DeleteLocalRef(jdefaultValue);

    thread.Detach();

    return 0;
}

static int GetIntSharedPreferences(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);

    dmAndroid::ThreadAttacher thread;
    JNIEnv* env = thread.GetEnv();

    const char* storageName = (char*)luaL_checkstring(L, 1);
    jstring jstorageName = env->NewStringUTF(storageName);

    const char* key = (char*)luaL_checkstring(L, 2);
    jstring jkey = env->NewStringUTF(key);

    jint jdefaultValue = static_cast<jint>(luaL_checkint(L, 3));

    AndroidJavaObject instance;
    GatJavaCoreInstance(env, &instance);
    jmethodID method = env->GetMethodID(instance.cls, "getIntSharedPreferences", "(Landroid/app/Activity;Ljava/lang/String;Ljava/lang/String;I)I");
    int value = (int)env->CallIntMethod(instance.obj, method, dmGraphics::GetNativeAndroidActivity(), jstorageName, jkey, jdefaultValue);
    lua_pushinteger(L, value);

    env->DeleteLocalRef(jstorageName);
    env->DeleteLocalRef(jkey);

    thread.Detach();

    return 1;
}

static int SetIntSharedPreferences(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);

    dmAndroid::ThreadAttacher thread;
    JNIEnv* env = thread.GetEnv();

    const char* storageName = (char*)luaL_checkstring(L, 1);
    jstring jstorageName = env->NewStringUTF(storageName);

    const char* key = (char*)luaL_checkstring(L, 2);
    jstring jkey = env->NewStringUTF(key);

    jint jvalue = static_cast<jint>(luaL_checkint(L, 3));

    AndroidJavaObject instance;
    GatJavaCoreInstance(env, &instance);
    jmethodID method = env->GetMethodID(instance.cls, "setIntSharedPreferences", "(Landroid/app/Activity;Ljava/lang/String;Ljava/lang/String;I)V");
    env->CallVoidMethod(instance.obj, method, dmGraphics::GetNativeAndroidActivity(), jstorageName, jkey, jvalue);

    env->DeleteLocalRef(jstorageName);
    env->DeleteLocalRef(jkey);
    
    thread.Detach();

    return 0;
}

static const luaL_reg Module_methods[] =
{
    {"connect", Connect},
    {"log_verbose", LogVerbose},
    {"log_debug", LogDebug},
    {"log_info", LogInfo},
    {"log_warning", LogWarning},
    {"log_error", LogError},
    {"show_toast", ShowToast},
    {"copy_to_clipboard", CopyToClipboard},
    {"get_from_clipboard", GetFromClipboard},
    {"get_string_resources", GetStringResources},
    {"get_string_shared_preferences", GetStringSharedPreferences},
    {"set_string_shared_preferences", SetStringSharedPreferences},
    {"get_int_shared_preferences", GetIntSharedPreferences},
    {"set_int_shared_preferences", SetIntSharedPreferences},
    {0, 0}
};

#else

static const luaL_reg Module_methods[] =
{
    {0, 0}
};

#endif

static void LuaInit(lua_State* L)
{
    int top = lua_gettop(L);
    luaL_register(L, MODULE_NAME, Module_methods);
    lua_pop(L, 1);
    assert(top == lua_gettop(L));
}

static dmExtension::Result AppInitializeMyExtension(dmExtension::AppParams* params)
{
    return dmExtension::RESULT_OK;
}

static dmExtension::Result InitializeMyExtension(dmExtension::Params* params)
{
    LuaInit(params->m_L);
    InitDefoldPlayer();
    InitRuStoreCallbacks();
    
    return dmExtension::RESULT_OK;
}

static dmExtension::Result AppFinalizeMyExtension(dmExtension::AppParams* params)
{
    return dmExtension::RESULT_OK;
}

static dmExtension::Result UpdateMyExtension(dmExtension::Params* params)
{

// #if defined(DM_PLATFORM_ANDROID)
//     dmLogInfo("UpdateMyExtension");
// #endif

    // auto _callbacks = ChannelCallbackManager::Instance()->FindLuaCallbacksByChannel("rustore_check_purchases_available_success");
    // for (const auto& callback : _callbacks)
    // {
    //     dmLogInfo("callback channel = %s", "rustore_check_purchases_available_success");
    // }

    auto queue = QueueCallbackManager::Instance()->GetExexuteQueueCallback();
    
    while (!queue.empty())
    {
        auto item = queue.front();
        queue.pop();

        const char* channel = item->channel.c_str();
        const char* value = item->value.c_str();

#if defined(DM_PLATFORM_ANDROID)
        dmLogInfo("queue channel = %s", channel);
        dmLogInfo("queue channel = %s", value);
#endif

        auto _callbacks = ChannelCallbackManager::Instance()->FindLuaCallbacksByChannel(channel);
        for (const auto& callback : _callbacks)
        {

#if defined(DM_PLATFORM_ANDROID)
        dmLogInfo("callback channel = %s", channel);
#endif
            lua_State* L = dmScript::GetCallbackLuaContext(callback);

            DM_LUA_STACK_CHECK(L, 0);

            if (!dmScript::SetupCallback(callback)) continue;

#if defined(DM_PLATFORM_ANDROID)
            dmLogInfo("callback value send = %s", value);
#endif

#if defined(DM_PLATFORM_ANDROID)

            dmAndroid::ThreadAttacher thread;
            JNIEnv* env = thread.GetEnv();

            if (strcmp(channel, "rustore_on_get_purchases_success") == 0) {

                jclass cls = dmAndroid::LoadClass(env, "ru.rustore.defold.core.RuStoreJsonConverter");
                jmethodID convertMethod = env->GetStaticMethodID(cls, "convertPurchasesDetails", "(Ljava/lang/String;)Ljava/lang/String;");
                jstring jvalue = env->NewStringUTF(value);
                jstring result = (jstring) env->CallStaticObjectMethod(cls, convertMethod, jvalue);
                const char *ctext = env->GetStringUTFChars(result, nullptr);

                dmLogInfo("rustore_on_get_purchases_success callback new value send = %s", ctext);
                
                lua_pushstring(L, ctext);
                lua_getglobal(L, "json");           // stack: json_str, json
                lua_getfield(L, -1, "decode");      // stack: json_str, json, json.decode
                lua_insert(L, -3);                  // stack: json.decode, json, json_str
                lua_pop(L, 1);                      // stack: json.decode, json_str
                lua_call(L, 1, 1);                  // stack: table

                dmScript::PCall(L, 2, 0); // self + # user arguments

                dmScript::TeardownCallback(callback);
            } else if (strcmp(channel, "rustore_on_purchase_product_failure") == 0) {

                lua_pushstring(L, value);

                dmScript::PCall(L, 2, 0); // self + # user arguments

                dmScript::TeardownCallback(callback);
            } else if (strcmp(channel, "rustore_on_purchase_product_success") == 0) {

                jclass cls = dmAndroid::LoadClass(env, "ru.rustore.defold.core.RuStoreJsonConverter");
                jmethodID convertMethod = env->GetStaticMethodID(cls, "convertPurchaseDetails", "(Ljava/lang/String;)Ljava/lang/String;");
                jstring jvalue = env->NewStringUTF(value);
                jstring result = (jstring) env->CallStaticObjectMethod(cls, convertMethod, jvalue);
                const char *ctext = env->GetStringUTFChars(result, nullptr);
                
                dmLogInfo("rustore_on_purchase_product_success callback new value send = %s", ctext);

                lua_pushstring(L, ctext);
                lua_getglobal(L, "json");           // stack: json_str, json
                lua_getfield(L, -1, "decode");      // stack: json_str, json, json.decode
                lua_insert(L, -3);                  // stack: json.decode, json, json_str
                lua_pop(L, 1);                      // stack: json.decode, json_str
                lua_call(L, 1, 1);                  // stack: table

                dmScript::PCall(L, 2, 0); // self + # user arguments

                dmScript::TeardownCallback(callback);

            } else if (strcmp(channel, "rustore_on_get_products_success") == 0) {

                jclass cls = dmAndroid::LoadClass(env, "ru.rustore.defold.core.RuStoreJsonConverter");
                jmethodID convertMethod = env->GetStaticMethodID(cls, "convertProductDetails", "(Ljava/lang/String;)Ljava/lang/String;");
                jstring jvalue = env->NewStringUTF(value);
                jstring result = (jstring) env->CallStaticObjectMethod(cls, convertMethod, jvalue);
                const char *ctext = env->GetStringUTFChars(result, nullptr);

                dmLogInfo("rustore_on_get_products_success callback new value send = %s", ctext);

                lua_pushstring(L, ctext);
                lua_getglobal(L, "json");           // stack: json_str, json
                lua_getfield(L, -1, "decode");      // stack: json_str, json, json.decode
                lua_insert(L, -3);                  // stack: json.decode, json, json_str
                lua_pop(L, 1);                      // stack: json.decode, json_str
                lua_call(L, 1, 1);                  // stack: table

                dmScript::PCall(L, 2, 0); // self + # user arguments

                dmScript::TeardownCallback(callback);
            } else {
                /* code */
                lua_pushstring(L, value);
                dmScript::PCall(L, 2, 0); // self + # user arguments

                dmScript::TeardownCallback(callback);
            }            

            thread.Detach();
#endif
            //lua_pushstring(L, channel);
            //lua_pushstring(L, value);


            // dmScript::PCall(L, 2, 0); // self + # user arguments

            // dmScript::TeardownCallback(callback);
        }
    }

    return dmExtension::RESULT_OK;
}

DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, AppInitializeMyExtension, AppFinalizeMyExtension, InitializeMyExtension, UpdateMyExtension, nullptr, nullptr)

extern "C"
{
    JNIEXPORT jobject JNICALL Java_ru_rustore_defold_core_wrappers_DefoldPlayerWrapper_NativeOnActivityRequest(JNIEnv* env, jobject obj)
    {
        return dmGraphics::GetNativeAndroidActivity();
    }
}
