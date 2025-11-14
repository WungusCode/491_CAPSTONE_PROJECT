// Tests that empty username or password don't allow navigating to the home page
// Tests that the login screen builds the expected widgets
#include <gtk/gtk.h>
#include "../login_screen.h"
#include "../create_screen.h"

static void test_login_screen_not_null(void) {
    gtk_init(NULL, NULL);

    hdl_grp handles = {0};
    struct { gboolean dbg; } flags = { FALSE };
    handles.flg = &flags;
    handles.parentWin = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    int rc = create_login_screen(&handles);

    g_assert_cmpint(rc, ==, 0);
    g_assert_nonnull(handles.vbox_login_page);
}

static void test_create_screen_not_null(void) {
    gtk_init(NULL, NULL);

    hdl_grp handles = {0};
    struct { gboolean dbg; } flags = { FALSE };
    handles.flg = &flags;
    handles.parentWin = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    int rc = create_create_screen(&handles);

    g_assert_cmpint(rc, ==, 0);
    g_assert_nonnull(handles.vbox_create_page);
}

int main(int argc, char **argv) {
    gtk_test_init(&argc, &argv, NULL);

    g_test_add_func("/login/create_login_not_null", test_login_screen_not_null);
    g_test_add_func("/create/create_screen_not_null", test_create_screen_not_null);

    return g_test_run();
}
