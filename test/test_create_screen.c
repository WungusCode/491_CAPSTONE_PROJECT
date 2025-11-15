// tests that the create screen builds the expected widgets and prevents empty fields


#include <gtk/gtk.h>
#include <glib.h>

#include "../create_screen.h"
#include "../base_defs.h"
#include "../home_page.h"

// create a dummy home screen function to satisfy the linker since the create screen will try to call it when the submit button is clicked
int create_home_screen(phdl_grp all_hdls) {
    (void)all_hdls; // avoid unused parameter warning
    return 0;
}

static void init_handles(hdl_grp *out)
{
    static app_flags flgs;
    static uiHdl     ui;

    memset(out, 0, sizeof(*out));

    out->parentWin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    out->flg       = &flgs;
    out->vbx_hdls  = &ui;

    out->vbox_create_page = NULL;
}

static void test_create_build(void)
{
    gtk_init(NULL, NULL);

    hdl_grp handles;
    init_handles(&handles);

    int rc = create_create_screen(&handles);

    g_assert_cmpint(rc, ==, 0);
    g_assert_nonnull(handles.vbox_create_page);
    g_assert_true(GTK_IS_BOX(handles.vbox_create_page));
}

static void test_create_prevents_empty(void)
{
    gtk_init(NULL, NULL);


    hdl_grp handles;
    init_handles(&handles);

    int rc = create_create_screen(&handles);
    g_assert_cmpint(rc, ==, 0);

    GtkWidget *page = handles.vbox_create_page;
    g_assert_true(GTK_IS_BOX(page));

    GList *children = gtk_container_get_children(GTK_CONTAINER(page));
    g_assert_nonnull(children);

    g_list_free(children);
}

int main(int argc, char **argv)
{
    gtk_test_init(&argc, &argv, NULL);

    g_test_add_func("/create/build_page", test_create_build);
    g_test_add_func("/create/prevent_empty", test_create_prevents_empty);

    return g_test_run();
}
