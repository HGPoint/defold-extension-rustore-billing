#define EXTENSION_NAME RuStoreBilling
// #define LIB_NAME "RuStoreBilling"
#define MODULE_NAME "iap"
#define LIB_NAME "iap"

#if defined(DM_PLATFORM_ANDROID)

#include <dmsdk/sdk.h>
#include <dmsdk/dlib/android.h>
#include "iap.h"
#include "iap_private.h"
#include <string>
#include <vector>
#include <dmsdk/sdk.h>

#include "../../extension-rustore-core/src/rustorecore.h"

struct IAP
{
    IAP()
    {
        memset(this, 0, sizeof(*this));
        m_autoFinishTransactions = true;
        m_buyStarted = true;
        m_ProviderId = PROVIDER_ID_RUSTORE;
    }
    bool            m_autoFinishTransactions;
    bool            m_buyStarted;
    int             m_ProviderId;
};

static IAP g_IAP;

static int Init(lua_State* L)
{
    dmLogInfo("Init START");
    //DM_LUA_STACK_CHECK(L, 0);

    dmAndroid::ThreadAttacher thread;
    JNIEnv* env = thread.GetEnv();

    jclass cls = dmAndroid::LoadClass(env, "ru.rustore.defold.billing.RuStoreBilling");
    jmethodID method = env->GetStaticMethodID(cls, "init", "(Landroid/app/Activity;Ljava/lang/String;Ljava/lang/String;Z)V");

    const char* id = "2063641305";//(char*)luaL_checkstring(L, 1);
    const char* scheme = "com.happygames.mergecafe.rustore.scheme";//(char*)luaL_checkstring(L, 2);

    dmLogInfo("Init id = %s", id);
    dmLogInfo("Init scheme = %s", scheme);

    jstring jid = env->NewStringUTF(id);
    jstring jscheme = env->NewStringUTF(scheme);
    jboolean jdebugLogs = false;

    int n = lua_gettop(L);
    if (n > 2) jdebugLogs = (jboolean)lua_toboolean(L, 3);

    env->CallStaticVoidMethod(cls, method, dmGraphics::GetNativeAndroidActivity(), jid, jscheme, jdebugLogs);

    env->DeleteLocalRef(jid);
    env->DeleteLocalRef(jscheme);

    thread.Detach();
    
    dmLogInfo("Init OK");

    return 0;
}

[[deprecated("This method is deprecated. This method only works for flows with an authorized user in RuStore.")]]
static int CheckPurchasesAvailability(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);

    dmAndroid::ThreadAttacher thread;
    JNIEnv* env = thread.GetEnv();

    jclass cls = dmAndroid::LoadClass(env, "ru.rustore.defold.billing.RuStoreBilling");
    jmethodID method = env->GetStaticMethodID(cls, "checkPurchasesAvailability", "()V");

    env->CallStaticVoidMethod(cls, method);

    thread.Detach();
    
    return 0;
}

static void SetAuthorizationStatus()
{
    dmLogInfo("SetAuthorizationStatus");
}

static int GetAuthorizationStatus(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);

    dmAndroid::ThreadAttacher thread;
    JNIEnv* env = thread.GetEnv();

    jclass cls = dmAndroid::LoadClass(env, "ru.rustore.defold.billing.RuStoreBilling");
    jmethodID method = env->GetStaticMethodID(cls, "getAuthorizationStatus", "()V");

    env->CallStaticVoidMethod(cls, method);

    thread.Detach();

    return 0;
}

static int GetProducts(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);

    std::vector<std::string> productIds;
    if (lua_istable(L, 1))
    {
        int tableSize = lua_objlen(L, 1);
        for (int i = 1; i <= tableSize; i++)
        {
            lua_rawgeti(L, 1, i);
            if (lua_isstring(L, -1))
            {
                const char* productId = lua_tostring(L, -1);
                dmLogInfo("productIds.push_back(%s)", productId);
                productIds.push_back(productId);
            }
            lua_pop(L, 1);
        }
    }

    dmAndroid::ThreadAttacher thread;
    JNIEnv* env = thread.GetEnv();

    jclass cls = dmAndroid::LoadClass(env, "ru.rustore.defold.billing.RuStoreBilling");
    jmethodID method = env->GetStaticMethodID(cls, "getProducts", "([Ljava/lang/String;)V");

    jobjectArray jproductIds = env->NewObjectArray(productIds.size(), env->FindClass("java/lang/String"), nullptr);
    for (int i = 0; i < productIds.size(); i++) {
        jstring jproductId = env->NewStringUTF(productIds[i].c_str());
        env->SetObjectArrayElement(jproductIds, i, jproductId);
        env->DeleteLocalRef(jproductId);
    }
    
    env->CallStaticVoidMethod(cls, method, jproductIds);

    env->DeleteLocalRef(jproductIds);
    
    thread.Detach();
    
    return 0;
}

static int PurchaseProduct(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);

    dmAndroid::ThreadAttacher thread;
    JNIEnv* env = thread.GetEnv();

    jclass cls = dmAndroid::LoadClass(env, "ru.rustore.defold.billing.RuStoreBilling");
    jmethodID method = env->GetStaticMethodID(cls, "purchaseProduct", "(Ljava/lang/String;Ljava/lang/String;)V");

    jclass cls2 = dmAndroid::LoadClass(env, "ru.rustore.defold.billing.RuStoreIntentFilterActivity");
    jmethodID getUUIDMethod = env->GetStaticMethodID(
        dmAndroid::LoadClass(env, "ru.rustore.defold.billing.RuStoreIntentFilterActivity"), 
        "getUUID", 
        "()Ljava/lang/String;"
    );
    jstring juuid = (jstring) env->CallStaticObjectMethod(cls2, getUUIDMethod);
    const char *uuid = env->GetStringUTFChars(juuid, nullptr);

    const char* productId = (char*)luaL_checkstring(L, 1);
    jstring jproductId = env->NewStringUTF(productId);


    dmLogInfo("PurchaseProduct productId = %s", productId);

    //jstring jparams;
    
    // int n = lua_gettop(L);
    // if (n > 1) {
    //     const char* params = (char*)luaL_checkstring(L, 2);
    //     jparams = env->NewStringUTF(params);
    // } else {
    //     jparams = env->NewStringUTF("");
    // }

    std::string jsonString = "{ \"orderId\":\"" + std::string(uuid) + "\", \"quantity\":1, \"payload\":\"\" }";

    jstring jparams = env->NewStringUTF(jsonString.c_str());

    //jparams = env->NewStringUTF("{ \"orderId\":\"%s\", \"quantity\":1, \"payload\":\"\" }", uuid);
    dmLogInfo("PurchaseProduct jparams = { \"orderId\":\"%s\", \"quantity\":1, \"payload\":\"\" }", uuid);

    env->CallStaticVoidMethod(cls, method, jproductId, jparams);

    env->DeleteLocalRef(jproductId);
    env->DeleteLocalRef(jparams);
    
    thread.Detach();

    return 0;
}

static int GetPurchases(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);

    dmAndroid::ThreadAttacher thread;
    JNIEnv* env = thread.GetEnv();

    jclass cls = dmAndroid::LoadClass(env, "ru.rustore.defold.billing.RuStoreBilling");
    jmethodID method = env->GetStaticMethodID(cls, "getPurchases", "()V");

    env->CallStaticVoidMethod(cls, method);

    thread.Detach();

    return 0;
}

static int ConfirmPurchaseById(jstring jproductId)
{
    dmAndroid::ThreadAttacher thread;
    JNIEnv* env = thread.GetEnv();

    jclass cls = dmAndroid::LoadClass(env, "ru.rustore.defold.billing.RuStoreBilling");
    jmethodID method = env->GetStaticMethodID(cls, "confirmPurchase", "(Ljava/lang/String;)V");

    env->CallStaticVoidMethod(cls, method, jproductId);
    
    thread.Detach();

    return 0;
}

const char* get_string_from_table(lua_State* L, const char* fieldName) {
    // Проверяем количество аргументов
    if (lua_gettop(L) != 1) {
        dmLogInfo("Ожидается 1 аргумент: таблица");
        return nullptr;
    }

    // Проверяем тип аргумента
    if (!lua_istable(L, 1)) {
        dmLogInfo("Аргумент должен быть таблицей");
        return nullptr;
    }

    dmLogInfo("Получаем значение по указанному полю %s", fieldName);
    // Получаем значение по указанному полю
    lua_getfield(L, 1, fieldName);
    
    // Проверяем, что значение является строкой
    if (lua_isstring(L, -1)) {
        const char* result = lua_tostring(L, -1);
        lua_pop(L, 1); // Очищаем стек
        return result;
    }
    
    // Если значение не является строкой
    dmLogInfo("Значение по ключу %s не является строкой", fieldName);
    return nullptr;
}

int log_table(lua_State* L) {
    // Проверяем тип параметра
    if (lua_istable(L, 1)) {
        lua_pushnil(L);  // Первый ключ
        
        while (lua_next(L, 1)) {
            // Получаем ключ и значение
            const char* key = lua_isstring(L, -2) ? lua_tostring(L, -2) : "non-string key";
            const char* value = lua_isstring(L, -1) ? lua_tostring(L, -1) : "non-string value";
            
            dmLogInfo("Ключ: %s, Значение: %s\n", key, value);
            
            lua_pop(L, 1);  // Удаляем значение, оставляем ключ для следующего вызова lua_next
        }
    } else {
        printf("Ошибка: параметр не является таблицей\n");
    }
    return 0;
}



static int ConfirmPurchase(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);

    dmAndroid::ThreadAttacher thread;
    JNIEnv* env = thread.GetEnv();

    jclass cls = dmAndroid::LoadClass(env, "ru.rustore.defold.billing.RuStoreBilling");
    jmethodID method = env->GetStaticMethodID(cls, "confirmPurchase", "(Ljava/lang/String;)V");

    const char* productId = (char*)luaL_checkstring(L, 1);
    jstring jproductId = env->NewStringUTF(productId);

    env->CallStaticVoidMethod(cls, method, jproductId);

    env->DeleteLocalRef(jproductId);
    
    thread.Detach();

    return 0;
}

static int DeletePurchase(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);

    dmAndroid::ThreadAttacher thread;
    JNIEnv* env = thread.GetEnv();

    jclass cls = dmAndroid::LoadClass(env, "ru.rustore.defold.billing.RuStoreBilling");
    jmethodID method = env->GetStaticMethodID(cls, "deletePurchase", "(Ljava/lang/String;)V");

    const char* productId = (char*)luaL_checkstring(L, 1);
    jstring jproductId = env->NewStringUTF(productId);

    env->CallStaticVoidMethod(cls, method, jproductId);

    env->DeleteLocalRef(jproductId);
    
    thread.Detach();

    return 0;
}

static int GetPurchaseInfo(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);

    dmAndroid::ThreadAttacher thread;
    JNIEnv* env = thread.GetEnv();

    jclass cls = dmAndroid::LoadClass(env, "ru.rustore.defold.billing.RuStoreBilling");
    jmethodID method = env->GetStaticMethodID(cls, "getPurchaseInfo", "(Ljava/lang/String;)V");

    const char* productId = (char*)luaL_checkstring(L, 1);
    jstring jproductId = env->NewStringUTF(productId);

    env->CallStaticVoidMethod(cls, method, jproductId);

    env->DeleteLocalRef(jproductId);
    
    thread.Detach();

    return 0;
}

static int SetTheme(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);

    dmAndroid::ThreadAttacher thread;
    JNIEnv* env = thread.GetEnv();

    jclass cls = dmAndroid::LoadClass(env, "ru.rustore.defold.billing.RuStoreBilling");
    jmethodID method = env->GetStaticMethodID(cls, "setTheme", "(I)V");

    jint jthemeCode = static_cast<jint>luaL_checkint(L, 1);

    env->CallStaticVoidMethod(cls, method, jthemeCode);

    thread.Detach();

    return 0;
}

[[deprecated("This method is deprecated. Error handling must be performed on the application side.")]]
static int SetErrorHandling(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);

    dmAndroid::ThreadAttacher thread;
    JNIEnv* env = thread.GetEnv();

    jclass cls = dmAndroid::LoadClass(env, "ru.rustore.defold.billing.RuStoreBilling");
    jmethodID method = env->GetStaticMethodID(cls, "setErrorHandling", "(Z)V");

    jboolean jvalue = (jboolean)lua_toboolean(L, 1);

    env->CallStaticVoidMethod(cls, method, jvalue);

    thread.Detach();

    return 0;
}

static int IAP_Buy(lua_State* L)
{
    dmLogInfo("IAP_Buy");

    DM_LUA_STACK_CHECK(L, 0);

    bool auth = GetCoreAuthorizationStatus();
    if(!auth){
        GetPurchases(L);
        CallBackCancelPurchase();
        return 0;
    }
    
    g_IAP.m_buyStarted = true;

    const char* productId = (char*)luaL_checkstring(L, 1);
    dmLogInfo("IAP_Buy productId = %s", productId);

    PurchaseProduct(L);

    return 0;
}

static int IAP_Finish(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);

    dmLogInfo("IAP_Finish");

    luaL_checktype(L, 1, LUA_TTABLE);

    lua_getfield(L, -1, "state");
    if (lua_isnumber(L, -1))
    {
        if(lua_tointeger(L, -1) != TRANS_STATE_PURCHASED)
        {
            dmLogError("Invalid transaction state (must be iap.TRANS_STATE_PURCHASED).");
            lua_pop(L, 1);
            return 0;
        }
    }
    lua_pop(L, 1);

    lua_getfield(L, -1, "receipt");
    if (!lua_isstring(L, -1)) {
        dmLogError("Transaction error. Invalid transaction data, does not contain 'receipt' key.");
        lua_pop(L, 1);
    }
    else
    {
        const char * purchaseId = lua_tostring(L, -1);
        lua_pop(L, 1);

        dmAndroid::ThreadAttacher thread;
        JNIEnv* env = thread.GetEnv();

        jclass cls = dmAndroid::LoadClass(env, "ru.rustore.defold.billing.RuStoreBilling");

        if (!cls) {
            dmLogInfo("Не удалось найти класс ru.rustore.defold.billing.RuStoreBilling");
            return 0;
        }

        jmethodID method = env->GetStaticMethodID(cls, "confirmPurchase", "(Ljava/lang/String;)V");

        jstring jpurchaseId = env->NewStringUTF(purchaseId);

        env->CallStaticVoidMethod(cls, method, jpurchaseId);

        env->DeleteLocalRef(jpurchaseId);

        thread.Detach();
    }

    return 0;
}

static int IAP_Acknowledge(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);

    dmLogInfo("IAP_Acknowledge");

    return 0;
}

static int IAP_Restore(lua_State* L)
{
    dmLogInfo("IAP_Restore");
    DM_LUA_STACK_CHECK(L, 1);

    return 1;
}

static int IAP_SetListener(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    
    dmAndroid::ThreadAttacher thread;
    JNIEnv* env = thread.GetEnv();

    dmScript::LuaCallbackInfo* callback = dmScript::CreateCallback(L, 1);

    ConnectCallback("rustore_on_purchase_product_success", callback);
    ConnectCallback("rustore_on_purchase_product_failure", callback);
    ConnectCallback("rustore_on_get_purchases_success", callback);
    ConnectCallback("rustore_on_get_purchases_failure", callback);

    bool auth = GetCoreAuthorizationStatus();
    if(auth){
        dmLogInfo("GetCoreAuthorizationStatus == true");
        GetPurchases(L);
    } else {
        dmLogInfo("GetCoreAuthorizationStatus == false");
    }
    
    // jstring jproductId = env->NewStringUTF("bc2f3ee3-dd55-48cc-9232-e4a88cc77f56");
    // ConfirmPurchaseById(jproductId);
    
    return 0;
}

static int IAP_GetProviderId(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    
    dmLogInfo("IAP_GetProviderId %i", g_IAP.m_ProviderId);

    lua_pushinteger(L, g_IAP.m_ProviderId);

    return 1;
}

static int IAP_ProcessPendingTransactions(lua_State* L)
{
    dmLogInfo("IAP_ProcessPendingTransactions");
    DM_LUA_STACK_CHECK(L, 0);
    
    return 0;
}

static int IAP_List(lua_State* L)
{
    dmLogInfo("IAP_List");
    
    DM_LUA_STACK_CHECK(L, 0);

    dmScript::LuaCallbackInfo* callback = dmScript::CreateCallback(L, 2);

    ConnectCallback("rustore_on_get_products_success", callback);
    GetProducts(L);
    
    return 0;
}

// Functions exposed to Lua
static const luaL_reg Module_methods[] =
{
    {"init", Init},
    {"check_purchases_availability", CheckPurchasesAvailability},
    {"get_authorization_status", GetAuthorizationStatus},
    {"get_products", GetProducts},
    {"purchase_product", PurchaseProduct},
    {"get_purchases", GetPurchases},
    {"confirm_purchase", ConfirmPurchase},
    {"delete_purchase", DeletePurchase},
    {"get_purchase_info", GetPurchaseInfo},
    {"set_theme", SetTheme},
    {"set_error_handling", SetErrorHandling},
    //
    {"list", IAP_List},
    {"buy", IAP_Buy},
    {"finish", IAP_Finish},
    {"acknowledge", IAP_Acknowledge},
    {"restore", IAP_Restore},
    {"set_listener", IAP_SetListener},
    {"get_provider_id", IAP_GetProviderId},
    {"process_pending_transactions", IAP_ProcessPendingTransactions},
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
    IAP_PushConstants(L);

	Init(L);
    GetAuthorizationStatus(L);

    lua_pop(L, 1);
    assert(top == lua_gettop(L));
}

static dmExtension::Result AppInitializeBillingExtension(dmExtension::AppParams* params)
{
    dmLogInfo("AppInitializeBillingExtension");
    return dmExtension::RESULT_OK;
}

static dmExtension::Result InitializeBillingExtension(dmExtension::Params* params)
{
    LuaInit(params->m_L);
    dmLogInfo("Registered %s Extension", MODULE_NAME);
    return dmExtension::RESULT_OK;
}

static dmExtension::Result AppFinalizeBillingExtension(dmExtension::AppParams* params)
{
    dmLogInfo("AppFinalizeBillingExtension");
    return dmExtension::RESULT_OK;
}

static dmExtension::Result FinalizeBillingExtension(dmExtension::Params* params)
{
    dmLogInfo("FinalizeBillingExtension");
    return dmExtension::RESULT_OK;
}

static dmExtension::Result OnUpdateBillingExtension(dmExtension::Params* params)
{
    //dmLogInfo("OnUpdateBillingExtension");
    return dmExtension::RESULT_OK;
}

static void OnEventBillingExtension(dmExtension::Params* params, const dmExtension::Event* event)
{
    switch(event->m_Event)
    {
        case dmExtension::EVENT_ID_ACTIVATEAPP:
        {
            dmLogInfo("OnEventBillingExtension - EVENT_ID_ACTIVATEAPP");
            bool auth = GetCoreAuthorizationStatus();
            if(auth && g_IAP.m_buyStarted){
                dmLogInfo("OnEventBillingExtension - EVENT_ID_ACTIVATEAPP - getPurchases");
                dmAndroid::ThreadAttacher thread;
                JNIEnv* env = thread.GetEnv();

                jclass cls = dmAndroid::LoadClass(env, "ru.rustore.defold.billing.RuStoreBilling");
                jmethodID method = env->GetStaticMethodID(cls, "getPurchases", "()V");

                env->CallStaticVoidMethod(cls, method);
            }
        }
        break;
        case dmExtension::EVENT_ID_DEACTIVATEAPP:
        dmLogInfo("OnEventBillingExtension - EVENT_ID_DEACTIVATEAPP");
        break;
        case dmExtension::EVENT_ID_ICONIFYAPP:
        dmLogInfo("OnEventBillingExtension - EVENT_ID_ICONIFYAPP");
        break;
        case dmExtension::EVENT_ID_DEICONIFYAPP:
        dmLogInfo("OnEventBillingExtension - EVENT_ID_DEICONIFYAPP");
        break;
        default:
        dmLogWarning("OnEventBillingExtension - Unknown event id");
        break;
    }
}

// Defold SDK uses a macro for setting up extension entry points:
//
// DM_DECLARE_EXTENSION(symbol, name, app_init, app_final, init, update, on_event, final)

// MyExtension is the C++ symbol that holds all relevant extension data.
// It must match the name field in the `ext.manifest`
DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, AppInitializeBillingExtension, AppFinalizeBillingExtension, InitializeBillingExtension, OnUpdateBillingExtension, OnEventBillingExtension, FinalizeBillingExtension)
