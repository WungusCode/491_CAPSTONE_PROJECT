// Tests that empty username or password don't allow navigating to the home page
// Tests that the login screen builds the expected widgets
#include <gtk/gtk.h>
#include <glib.h>

#include "../login_screen.h"
#include "../create_screen.h"
#include "../home_page.h"

// create a dummy home screen function to satisfy the linker since the login screen will try to call it when the submit button is clicked
int create_home_screen(phdl_grp all_hdls) {
    (void)all_hdls; // avoid unused parameter warning
    return 0;
}

// changed function to initialize a real hdl_grp struct with the necessary fields for testing from base_defs.h
static void init_handles(hdl_grp *out)
{
    static app_flags flgs;   // lives for the whole test run
    static uiHdl     ui;     // same

    memset(out, 0, sizeof(*out));

    out->parentWin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    out->flg       = &flgs;
    out->vbx_hdls  = &ui;

    // start with these pages null; the create_* functions will fill them in
    out->vbox_start_page  = NULL;
    out->vbox_login_page  = NULL;
    out->vbox_create_page = NULL;
}


static void test_login_screen_not_null(void) {
    gtk_init(NULL, NULL);

    hdl_grp handles;
    init_handles(&handles);

    int rc = create_login_screen(&handles);

    g_assert_cmpint(rc, ==, 0);
    g_assert_nonnull(handles.vbox_login_page);
    g_assert_true(GTK_IS_BOX(handles.vbox_login_page));
}

static void test_create_screen_not_null(void) {

    gtk_init(NULL, NULL);

    hdl_grp handles;
    init_handles(&handles);

    int rc = create_create_screen(&handles);

    g_assert_cmpint(rc, ==, 0);
    g_assert_nonnull(handles.vbox_create_page);
    g_assert_true(GTK_IS_BOX(handles.vbox_create_page));
}

// test that the login screen has the expected buttons and that they are hooked up to the correct widgets in the uiHdl struct
static void test_login_buttons_hooked(void)
{
    gtk_init(NULL, NULL);

    hdl_grp handles;
    init_handles(&handles);

    int rc = create_login_screen(&handles);
    g_assert_cmpint(rc, ==, 0);

    // these field names come from the uiHdl in base_defs.h
    g_assert_nonnull(handles.vbx_hdls);
    g_assert_nonnull(handles.vbx_hdls->lp_back_btn);
    g_assert_nonnull(handles.vbx_hdls->lp_create_btn);
    g_assert_nonnull(handles.vbx_hdls->lp_submit_btn);

    g_assert_true(GTK_IS_BUTTON(handles.vbx_hdls->lp_back_btn));
    g_assert_true(GTK_IS_BUTTON(handles.vbx_hdls->lp_create_btn));
    g_assert_true(GTK_IS_BUTTON(handles.vbx_hdls->lp_submit_btn));
}

int main(int argc, char **argv) {

    gtk_test_init(&argc, &argv, NULL);

    g_test_add_func("/login/create_login_not_null", test_login_screen_not_null);
    g_test_add_func("/create/create_screen_not_null", test_create_screen_not_null);
    g_test_add_func("/login/buttons_hooked",          test_login_buttons_hooked);

    return g_test_run();
}
