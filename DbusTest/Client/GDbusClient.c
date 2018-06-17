/**********************************************************************************************
*
*   Filename: GDbusClient.c
*
*   Author: theArcticOcean - wei_yang1994@163.com
*
*   Create: 2018-06-17 11:30:37
**********************************************************************************************/

#include <glib.h>
#include <unistd.h>
#include <stdlib.h>
#include "DbusTest_code.h"

static gboolean onValueChanged
    (
    OrgExampleQdbusWtest *object,
    double newValue,
    gpointer user_data
    )
{
    g_print( "onValueChanged, get value: %lf\n", newValue );
    return TRUE;
}

int main( int argc, char *argv[] )
{
    OrgExampleQdbusWtest *proxy = NULL;
    GError *error = NULL;
    GMainLoop* loop = NULL;

    proxy = org_example_qdbus_wtest_proxy_new_for_bus_sync(
            G_BUS_TYPE_SESSION,
            G_DBUS_PROXY_FLAGS_NONE,
            "org.example.qdbus.wtest",
            "/com/examples/qdbus/wtest",
            NULL,
            &error );
    if( NULL == proxy )
    {
        g_print( "proxy init failed: %s", error->message );
        exit( 0 );
    }
    // connect step
    g_signal_connect( proxy, "value-changed", G_CALLBACK( onValueChanged ), NULL );

    // set new value.
    org_example_qdbus_wtest_call_set_value_sync( proxy, 20, NULL, &error );
    loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run( loop );
    
    // Decreases the reference count of object . When its reference count drops to 0, the object is finalized (i.e. its memory is freed).
    g_object_unref( proxy );
    return 0;
}
