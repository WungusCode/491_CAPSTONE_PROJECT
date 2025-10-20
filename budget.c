#include <stdio.h>
#include<gtk/gtk.h>
#include <stdlib.h>

#include "base_defs.h"
#include "indent_print.h"

#include "budget.h"
#include "home_page.h"

long g_budget_value = 0;
char *g_budget_string = NULL;

static void on_close_clicked(GtkButton *btn, gpointer user_data) {
    GtkWidget *window = GTK_WIDGET(user_data);
    if (window) gtk_widget_destroy(window);
}

void update_budget_string() {
    if (g_budget_string != NULL) {
        g_free(g_budget_string);
    }
    g_budget_string = g_strdup_printf("%ld", g_budget_value); 
}

void enter_budget(GtkWidget *button, gpointer data) {
    GtkWidget *entry = GTK_WIDGET(data); 
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
    long new_budget = strtol(text, NULL, 10);
    g_budget_value = new_budget; 
    update_budget_string();

    g_print("Budget set to: %ld\n", g_budget_value);
}

GtkWidget* create_budget_page(void) {
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Set Budget");
    gtk_window_set_default_size(GTK_WINDOW(window), WIN_W, WIN_H);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 16);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget *frame_budget = gtk_frame_new("Set Budget");
    GtkWidget *box_budget = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(box_budget), 10);
    gtk_container_add(GTK_CONTAINER(frame_budget), box_budget);
    gtk_box_pack_start(GTK_BOX(vbox), frame_budget, FALSE, FALSE, 0);

    // Budget entry
    GtkWidget *btn_budget = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(btn_budget), "Budget");
    gtk_box_pack_start(GTK_BOX(box_budget), btn_budget, FALSE, FALSE, 0);

    // --- BUTTONS ---
    GtkWidget *btn_enter = gtk_button_new_with_label("Enter");
    g_signal_connect(btn_enter, "clicked", G_CALLBACK(enter_budget), btn_budget); 
    gtk_box_pack_start(GTK_BOX(box_budget), btn_enter, FALSE, FALSE, 0);

    GtkWidget *btn_close = gtk_button_new_with_label("Close");
    g_signal_connect_swapped(btn_close, "clicked", G_CALLBACK(on_close_clicked), window);
    gtk_box_pack_end(GTK_BOX(vbox), btn_close, FALSE, FALSE, 0);
  
    gtk_widget_show_all(window);

    return window;
}

void destroy_budget_page(GtkWidget *window) {
    if (window) {
        gtk_widget_destroy(window);
    }
    printf("[Budget] window destroyed\n");
}