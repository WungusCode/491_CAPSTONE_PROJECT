#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "base_defs.h"
#include "budget.h"
#include "home_page.h"

long g_budget_value = 0;
char *g_budget_string = NULL;

// --- Local UI references ---
#define BUDGET_MIN   0
#define BUDGET_MAX   100000
#define BUDGET_STEP  100

static GtkWidget *g_lbl_budget = NULL;
static GtkWidget *g_entry_budget = NULL;
static GtkWidget *g_scale_budget = NULL;

// --- Helpers ---
static void update_budget_string(void) {
    if (g_budget_string) {
        g_free(g_budget_string);
        g_budget_string = NULL;
    }
    g_budget_string = g_strdup_printf("%ld", g_budget_value);
}

static void update_budget_label(void) {
    if (!g_lbl_budget) return;
    char buf[128];
    snprintf(buf, sizeof(buf), "Current Budget: $%ld", g_budget_value);
    gtk_label_set_text(GTK_LABEL(g_lbl_budget), buf);
}

static void set_entry_text_from_value(long value) {
    if (!g_entry_budget) return;
    char buf[64];
    snprintf(buf, sizeof(buf), "%ld", value);
    gtk_entry_set_text(GTK_ENTRY(g_entry_budget), buf);
}

static void set_scale_from_value(long value) {
    if (!g_scale_budget) return;
    if (value < BUDGET_MIN) value = BUDGET_MIN;
    if (value > BUDGET_MAX) value = BUDGET_MAX;
    gtk_range_set_value(GTK_RANGE(g_scale_budget), (gdouble)value);
}

static void adopt_budget_value(long new_value, gboolean sync_entry, gboolean sync_scale, gboolean announce) {
    if (new_value < BUDGET_MIN) new_value = BUDGET_MIN;
    if (new_value > BUDGET_MAX) new_value = BUDGET_MAX;
    if (BUDGET_STEP > 1) {
        long r = new_value % BUDGET_STEP;
        if (r >= BUDGET_STEP / 2) new_value += (BUDGET_STEP - r);
        else new_value -= r;
    }

    g_budget_value = new_value;
    update_budget_string();
    update_budget_label();
    if (sync_entry) set_entry_text_from_value(g_budget_value);
    if (sync_scale) set_scale_from_value(g_budget_value);
    if (announce) g_print("[Budget] set to: %ld\n", g_budget_value);
}

// --- Navigation helper ---
static void switch_to_vbox(phdl_grp h, GtkWidget *new_vbox) {
    if (h->vbox_active) {
        gtk_widget_hide(h->vbox_active);
        gtk_container_remove(GTK_CONTAINER(h->parentWin), h->vbox_active);
    }
    gtk_container_add(GTK_CONTAINER(h->parentWin), new_vbox);
    h->vbox_active = new_vbox;
    gtk_widget_show_all(new_vbox);
}

// --- Callbacks ---
static void on_budget_done_clicked(GtkButton *btn, gpointer data) {
    phdl_grp h = (phdl_grp)data;
    if (h->vbox_home_page) {
        switch_to_vbox(h, h->vbox_home_page);
    }
}

static void on_save_clicked(GtkButton *btn, gpointer data) {
    g_print("[Budget] SAVE clicked: %ld (add DB save later)\n", g_budget_value);
}

static void on_scale_value_changed(GtkRange *range, gpointer user_data) {
    long val = (long) gtk_range_get_value(range);
    adopt_budget_value(val, TRUE, FALSE, FALSE);
}

static void on_entry_activate(GtkEntry *entry, gpointer user_data) {
    const gchar *text = gtk_entry_get_text(entry);
    char *endp = NULL;
    long val = strtol(text, &endp, 10);

    if (text == endp) {
        set_entry_text_from_value(g_budget_value);
        return;
    }
    adopt_budget_value(val, FALSE, TRUE, FALSE);
}

static void on_entry_changed(GtkEditable *editable, gpointer user_data) {
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(editable));
    char *endp = NULL;
    long val = strtol(text, &endp, 10);

    if (text != endp) {
        if (val < BUDGET_MIN) val = BUDGET_MIN;
        if (val > BUDGET_MAX) val = BUDGET_MAX;
        g_budget_value = val;
        update_budget_string();
        update_budget_label();
    }
}

// --- Public API: create_budget_page ---
int create_budget_page(phdl_grp pall_hdls) {
    if (!pall_hdls) return -1;

    if (pall_hdls->vbox_budget_page == NULL) {
        GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
        gtk_container_set_border_width(GTK_CONTAINER(vbox), 16);
        pall_hdls->vbox_budget_page = vbox;

        // Label
        g_lbl_budget = gtk_label_new(NULL);
        gtk_widget_set_halign(g_lbl_budget, GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(vbox), g_lbl_budget, FALSE, FALSE, 0);

        // Frame + content
        GtkWidget *frame_budget = gtk_frame_new("Set Budget");
        gtk_box_pack_start(GTK_BOX(vbox), frame_budget, FALSE, FALSE, 0);

        GtkWidget *box_budget = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
        gtk_container_set_border_width(GTK_CONTAINER(box_budget), 10);
        gtk_container_add(GTK_CONTAINER(frame_budget), box_budget);

        // Entry
        g_entry_budget = gtk_entry_new();
        gtk_entry_set_placeholder_text(GTK_ENTRY(g_entry_budget), "Enter budget (e.g., 2500)");
        gtk_box_pack_start(GTK_BOX(box_budget), g_entry_budget, FALSE, FALSE, 0);

        // Slider
        GtkAdjustment *adj = gtk_adjustment_new((gdouble)g_budget_value,(gdouble)BUDGET_MIN,(gdouble)BUDGET_MAX,(gdouble)BUDGET_STEP,(gdouble)(BUDGET_STEP * 10),0.0);
        g_scale_budget = gtk_scale_new(GTK_ORIENTATION_HORIZONTAL, adj);
        gtk_scale_set_draw_value(GTK_SCALE(g_scale_budget), TRUE);
        gtk_box_pack_start(GTK_BOX(box_budget), g_scale_budget, FALSE, FALSE, 0);

        // Buttons
        GtkWidget *btn_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
        gtk_box_pack_start(GTK_BOX(vbox), btn_row, FALSE, FALSE, 0);

        GtkWidget *btn_save = gtk_button_new_with_label("Save");
        GtkWidget *btn_back = gtk_button_new_with_label("Back");
        gtk_box_pack_start(GTK_BOX(btn_row), btn_save, FALSE, FALSE, 0);
        gtk_box_pack_end(GTK_BOX(btn_row), btn_back, FALSE, FALSE, 0);

        // Signals
        g_signal_connect(g_scale_budget, "value-changed", G_CALLBACK(on_scale_value_changed), NULL);
        g_signal_connect(g_entry_budget, "activate",      G_CALLBACK(on_entry_activate), NULL);
        g_signal_connect(g_entry_budget, "changed",       G_CALLBACK(on_entry_changed),  NULL);
        g_signal_connect(btn_save, "clicked", G_CALLBACK(on_save_clicked), NULL);
        g_signal_connect(btn_back, "clicked", G_CALLBACK(on_budget_done_clicked), pall_hdls);

        // Initial sync
        adopt_budget_value(g_budget_value, TRUE, TRUE, FALSE);
    }

    switch_to_vbox(pall_hdls, pall_hdls->vbox_budget_page);
    return 0;
}

