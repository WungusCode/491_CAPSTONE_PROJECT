// tests that the create screen builds the expected widgets and prevents empty fields


#include <gtk/gtk.h>
#include <glib.h>

#include "../create_screen.h"

extern gboolean is_blank(const char *input);

typedef struct {
    GtkWidget *parentWin;
    GtkWidget *vbox_create_page;
    struct { gboolean dbg; } flags;
    struct {
        GtkWidget *cp_submit_btn;
    } vbx_hdls;
} mock_hdl, *phdl_grp;

static void test_create_build(void)
{
    gtk_init(NULL, NULL);

    mock_hdl h = {0};
    h.parentWin = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    int rc = create_create_screen((phdl_grp)&h);

    g_assert_cmpint(rc, ==, 0);
    g_assert_nonnull(h.vbox_create_page);
    g_assert_true(GTK_IS_BOX(h.vbox_create_page));
}

static void test_create_prevents_empty(void)
{
    gtk_init(NULL, NULL);

    mock_hdl h = {0};
    h.parentWin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    create_create_screen((phdl_grp)&h);

    GtkWidget *btn = h.vbx_hdls.cp_submit_btn;
    GtkEntry *u = g_object_get_data(G_OBJECT(btn), "u");
    GtkEntry *p = g_object_get_data(G_OBJECT(btn), "p");

    gtk_entry_set_text(u, "");
    gtk_entry_set_text(p, "");

    gboolean blocked = is_blank(gtk_entry_get_text(u)) ||
                       is_blank(gtk_entry_get_text(p));

    g_assert_true(blocked);
}

int main(int argc, char **argv)
{
    gtk_test_init(&argc, &argv, NULL);

    g_test_add_func("/create/build_page", test_create_build);
    g_test_add_func("/create/prevent_empty", test_create_prevents_empty);

    return g_test_run();
}
