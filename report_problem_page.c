#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include "report_problem_page.h"

static void on_submit_clicked(GtkButton *btn, gpointer user_data) {
    GtkWidget **entries = (GtkWidget **)user_data;

    const char *subject = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const char *message = gtk_text_buffer_get_text(
        gtk_text_view_get_buffer(GTK_TEXT_VIEW(entries[1])),
        &(GtkTextIter){0}, &(GtkTextIter){0}, TRUE);

    if (strlen(subject) == 0 || strlen(message) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING,
                                                   GTK_BUTTONS_OK, "Please fill in both fields.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    // Use mailto link (simplest method)
    char cmd[1024];
    snprintf(cmd, sizeof(cmd),
             "xdg-email --subject \"%s\" --body \"%s\" your_email@example.com",
             subject, message);
    system(cmd);

    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_OK, "Problem report sent!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

GtkWidget* create_report_problem_page(GtkWindow *parent) {
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Report a Problem");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    gtk_window_set_transient_for(GTK_WINDOW(window), parent);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget *entry_subject = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_subject), "Subject");
    gtk_box_pack_start(GTK_BOX(vbox), entry_subject, FALSE, FALSE, 0);

    GtkWidget *text_message = gtk_text_view_new();
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_message), GTK_WRAP_WORD);
    gtk_box_pack_start(GTK_BOX(vbox), text_message, TRUE, TRUE, 0);

    GtkWidget *btn_submit = gtk_button_new_with_label("Submit Report");

    GtkWidget *entries[2] = { entry_subject, text_message };
    g_signal_connect(btn_submit, "clicked", G_CALLBACK(on_submit_clicked), entries);
    gtk_box_pack_start(GTK_BOX(vbox), btn_submit, FALSE, FALSE, 0);

    gtk_widget_show_all(window);
    return window;
}