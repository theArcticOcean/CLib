/**********************************************************************************************
*
*   Filename: GDbusServer.c
*
*   Author: theArcticOcean - wei_yang1994@163.com
*
*   Create: 2018-06-17 13:56:15
**********************************************************************************************/

#include "DbusTest_code.h"

OrgExampleQdbusWtest *adaptor;

struct IPrivate
{
    double data;
};
struct IPrivate iPrivate;

// Callback function on_handle_set_value has form
// which is deined in struct _OrgExampleQdbusWtestIface
static gboolean on_handle_set_value
    (
    OrgExampleQdbusWtest *object,
    GDBusMethodInvocation *invocation,
    gdouble arg_newValue
    )
{
    g_print( "on_handle_set_value, set value: %lf\n", arg_newValue );
    iPrivate.data = arg_newValue;
    org_example_qdbus_wtest_complete_set_value( object, invocation );
    org_example_qdbus_wtest_emit_value_changed( object, arg_newValue );
    return TRUE;
}

static void GBusAcquired_Callback
    (
    GDBusConnection *connection,
    const gchar *name,
    gpointer user_data
    )
{
    GError *error = NULL;
    g_print( "GBusAcquired_Callback, name is %s, user_data: %s\n", name, (char *)user_data );
    adaptor = org_example_qdbus_wtest_skeleton_new();    
    // connect step
    g_signal_connect( adaptor, "handle-set-value", G_CALLBACK( on_handle_set_value ), NULL );
    g_dbus_interface_skeleton_export( G_DBUS_INTERFACE_SKELETON( adaptor ), connection, "/com/examples/qdbus/wtest", &error );
    if( NULL != error )
    {
        g_print( "Failed to export object: %s\n", error->message );
        g_error_free( error );
    }
}

void GBusNameAcquired_Callback
    (
    GDBusConnection *connection,
    const gchar *name,
    gpointer user_data
    )
{
    g_print( "GBusNameAcquired_Callback, name: %s\n", name );
}

/**
If you always disconnect like this:

./build/Server/Server
GBusNameLost_Callback, connection: 0x0, name: org.example.qdbus.wtest, user_data: 0x0

you can set bus address environment variable.
export DBUS_SESSION_BUS_ADDRESS= unix:path=/run/dbus/session_bus_help
*/
void GBusNameLost_Callback
    (
    GDBusConnection *connection,
    const gchar *name,
    gpointer user_data
    )
{
    g_print( "GBusNameLost_Callback, connection: %p, name: %s, user_data: %p\n", connection, name, user_data );
}

int main()
{
    GMainLoop* loop = NULL;
    guint own_id = g_bus_own_name(
                    G_BUS_TYPE_SESSION,
                    "org.example.qdbus.wtest",
                    G_BUS_NAME_OWNER_FLAGS_NONE,
                    GBusAcquired_Callback,
                    GBusNameAcquired_Callback,
                    GBusNameLost_Callback,
                    NULL,
                    NULL
                    );
    
    loop = g_main_loop_new( NULL, FALSE );
    g_main_loop_run( loop );

    g_bus_unown_name( own_id );
    return 0;
}
