#include <gtk/gtk.h>
#include "base_defs.h"
#include "transact_page.h"
#include "pie_page.h"
#include "transaction_list_page.h"
#include "setting_page.h"

/* ---------- helper to center content ---------- */
static GtkWidget* center_in_page(GtkWidget *content) {
    GtkWidget *page = gtk_grid_new();
    GtkWidget *top = gtk_label_new(NULL), *bottom = gtk_label_new(NULL);
    GtkWidget *left = gtk_label_new(NULL), *right = gtk_label_new(NULL);

    gtk_widget_set_hexpand(top, TRUE);    gtk_widget_set_vexpand(top, TRUE);
    gtk_widget_set_hexpand(bottom, TRUE); gtk_widget_set_vexpand(bottom, TRUE);
    gtk_widget_set_hexpand(left, TRUE);   gtk_widget_set_vexpand(left, TRUE);
    gtk_widget_set_hexpand(right, TRUE);  gtk_widget_set_vexpand(right, TRUE);

    gtk_widget_set_hexpand(content, FALSE);
    gtk_widget_set_vexpand(content, FALSE);

    gtk_grid_attach(GTK_GRID(page), top,     0, 0, 3, 1);
    gtk_grid_attach(GTK_GRID(page), left,    0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(page), content, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(page), right,   2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(page), bottom,  0, 2, 3, 1);

    return page;
}

/* ---------- nav callbacks ---------- */
static void hide_home_page(phdl_grp h) {
    gtk_widget_hide(h->vbox_home_page);
    gtk_container_remove(GTK_CONTAINER(h->parentWin), h->vbox_home_page);
}

static void chart_clicked(GtkButton *b, gpointer data) {
    (void)b; phdl_grp h = (phdl_grp)data;
    hide_home_page(h);
    create_pie_chart_page(h);
}

static void transact_clicked(GtkButton *b, gpointer data) {
    (void)b; phdl_grp h = (phdl_grp)data;
    hide_home_page(h);
    create_transaction_page(h);
}

static void list_transact_clicked(GtkButton *b, gpointer data) {
    (void)b; phdl_grp h = (phdl_grp)data;
    hide_home_page(h);
    create_transaction_history_page(h);
}

static void on_settings_clicked(GtkButton *b, gpointer d) {
    (void)b; (void)d;
    create_setting_page();
}

/* ---------- screen builder ---------- */
int create_home_screen(phdl_grp h) {
    if (!h) return -1;
    g_printerr("%s built %s %s\n", __func__, __DATE__, __TIME__);

    if (h->vbox_home_page == NULL) {
        /* ----- content block ----- */
        GtkWidget *content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
        gtk_container_set_border_width(GTK_CONTAINER(content), 16);

        /* row 1: balance label */
        GtkWidget *row1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
        GtkWidget *label = gtk_label_new(" $3,261 ");
        gtk_widget_set_halign(label, GTK_ALIGN_CENTER);
        gtk_box_pack_start(GTK_BOX(row1), label, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(content), row1, FALSE, FALSE, 0);

        /* row 2: "Show more details" */
        GtkWidget *row2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
        GtkWidget *button = gtk_button_new_with_label("Show more details");
        gtk_widget_set_valign(button, GTK_ALIGN_CENTER);
        gtk_box_pack_start(GTK_BOX(row2), button, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(content), row2, FALSE, FALSE, 0);

        /* row 3: icon buttons */
        GtkWidget *row3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);

        /* chart */
        button = gtk_button_new_with_label("");
        GtkWidget *image = gtk_image_new_from_file("./resources/libreoffice-chart.png");
        gtk_button_set_always_show_image(GTK_BUTTON(button), TRUE);
        gtk_button_set_image(GTK_BUTTON(button), image);
        h->vbx_hdls->hp_chart_btn = button;
        g_object_set(button, "tooltip-text", "Click to display Pie Chart", NULL);
        g_signal_connect(button, "clicked", G_CALLBACK(chart_clicked), h);
        gtk_box_pack_start(GTK_BOX(row3), button, FALSE, FALSE, 0);

        /* transaction */
        button = gtk_button_new_with_label("");
        image = gtk_image_new_from_file("./resources/transaction_64x64.png");
        gtk_button_set_always_show_image(GTK_BUTTON(button), TRUE);
        gtk_button_set_image(GTK_BUTTON(button), image);
        h->vbx_hdls->hp_plus_trans_btn = button;
        g_object_set(button, "tooltip-text", "Click to add or delete a transaction", NULL);
        g_signal_connect(button, "clicked", G_CALLBACK(transact_clicked), h);
        gtk_box_pack_start(GTK_BOX(row3), button, FALSE, FALSE, 0);

        /* list */
        button = gtk_button_new_with_label("");
        image = gtk_image_new_from_file("./resources/list_transactions_64x64.png");
        gtk_button_set_always_show_image(GTK_BUTTON(button), TRUE);
        gtk_button_set_image(GTK_BUTTON(button), image);
        h->vbx_hdls->hp_list_trans_btn = button;
        g_object_set(button, "tooltip-text", "Click to show all transactions", NULL);
        g_signal_connect(button, "clicked", G_CALLBACK(list_transact_clicked), h);
        gtk_box_pack_start(GTK_BOX(row3), button, FALSE, FALSE, 0);

        /* settings */
        button = gtk_button_new_with_label("");
        image = gtk_image_new_from_file("./resources/settings.png");
        gtk_button_set_always_show_image(GTK_BUTTON(button), TRUE);
        gtk_button_set_image(GTK_BUTTON(button), image);
        g_signal_connect(button, "clicked", G_CALLBACK(on_settings_clicked), NULL);
        g_object_set(button, "tooltip-text", "Click to change app settings", NULL);
        gtk_box_pack_start(GTK_BOX(row3), button, FALSE, FALSE, 0);

        gtk_box_pack_start(GTK_BOX(content), row3, FALSE, FALSE, 0);

        /* wrap in center grid & attach */
        h->vbox_home_page = center_in_page(content);
        gtk_container_add(GTK_CONTAINER(h->parentWin), h->vbox_home_page);
        g_object_ref(h->vbox_home_page);
    } else {
        gtk_container_add(GTK_CONTAINER(h->parentWin), h->vbox_home_page);
    }

    gtk_widget_show_all(h->vbox_home_page);
    return 0;
}

/* wrapper */
int create_home_screen_rtn(phdl_grp *ph) {
    phdl_grp h2 = *ph;
    int rc = create_home_screen(h2);
    *ph = h2;
    return rc;
}
