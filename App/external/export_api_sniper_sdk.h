#ifndef EXPORT_API_SNIPER_SDK_H
#define EXPORT_API_SNIPER_SDK_H
#include  <stdint.h>

#define EXPORT_API_SNIPER_SDK
#if defined(__WIN32) || defined(__WIN64)
    #ifdef EXPORT_API_SNIPER_SDK
        #define EXPORT_API_SNIPER_SDK __declspec(dllexport)
    #else
        #define EXPORT_API_SNIPER_SDK __declspec(dllimport)
    #endif
#endif

typedef void(__cdecl *connected_callback)();


extern "C"
{
    EXPORT_API_SNIPER_SDK void connect_to_server_async_export(const char * server_host,const char * port,connected_callback callback);
    EXPORT_API_SNIPER_SDK void send_message_export(const char * message);

}

#endif