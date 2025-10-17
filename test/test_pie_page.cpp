#include <gtk/gtk.h>
#include <glib.h>
#include "pie_page.c"   // or better: include a header if available

// Mock structures
typedef struct {
    GtkWidget *parentWin;
    GtkWidget *vbox_chart_page;
    GtkWidget *vbox_home_page;
    struct {
        gboolean dbg;
    } *flg;
    struct {
        GtkWidget *cp_pie_done_btn;
    } *vbx_hdls;
} mock_hdl_grp, *phdl_grp;

static void test_create_pie_chart_page_not_null(void) {
    gtk_init(NULL, NULL); // Required before creating widgets

    mock_hdl_grp handles = {0};
    handles.parentWin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    struct { gboolean dbg; } flags = { TRUE };
    handles.flg = &flags;
    struct { GtkWidget *cp_pie_done_btn; } vbox_hdls = { NULL };
    handles.vbx_hdls = &vbox_hdls;

    // runs the function
    int rc = create_pie_chart_page(&handles);

    // Assertions
    g_assert_cmpint(rc, ==, 0);
    g_assert_nonnull(handles.vbox_chart_page);
    g_assert_true(GTK_IS_BOX(handles.vbox_chart_page));

    // chekcs to see fi  chart title label
    GList *children = gtk_container_get_children(GTK_CONTAINER(handles.vbox_chart_page));
    g_assert_nonnull(children);
    GtkWidget *first_child = GTK_WIDGET(children->data);
    g_assert_true(GTK_IS_LABEL(first_child));

    //  ckecks to see if pie chart widget exists had issues with this
    gboolean found_pie = FALSE;
    for (GList *l = children; l != NULL; l = l->next) {
        if (GTK_IS_DRAWING_AREA(l->data)) {
            found_pie = TRUE;
            break;
        }
    }
    g_assert_true(found_pie);

    g_list_free(children);
}

int main(int argc, char **argv) {
    gtk_test_init(&argc, &argv, NULL);
    g_test_add_func("/pie_page/create_chart_page_not_null", test_create_pie_chart_page_not_null);
    return g_test_run();
}