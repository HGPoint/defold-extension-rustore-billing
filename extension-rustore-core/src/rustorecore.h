#if defined(DM_PLATFORM_HTML5) || defined(DM_PLATFORM_ANDROID) || defined(DM_PLATFORM_IOS)

#ifndef RUSTORECORE_H
#define RUSTORECORE_H

#include <dmsdk/sdk.h>
#include <dmsdk/dlib/android.h>

static int Connect(lua_State* L);
int ConnectCallback(const char* channel, dmScript::LuaCallbackInfo* callback);

#endif  // RUSTORECORE_H

#endif // DM_PLATFORM_HTML5 || DM_PLATFORM_ANDROID || DM_PLATFORM_IOS