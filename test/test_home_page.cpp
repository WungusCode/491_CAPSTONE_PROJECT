#include <gtk/gtk.h>
#include <glib.h>
#include "home_page.c"   
#include "indent_print.h"


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