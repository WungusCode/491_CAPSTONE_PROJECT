#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include "report_problem_page.h"

static void on_submit_clicked(GtkButton *btn, gpointer user_data) {
    GtkWidget **entries = (GtkWidget **)user_data;

    const char *name = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const char *email = gtk_entry_get_text(GTK_ENTRY(entries[1]));

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(entries[2]));
    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);
    char *message = gtk_text_buffer_get_text(buffer, &start, &end, TRUE);

    if (strlen(name) == 0 || strlen(email) == 0 || strlen(message) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(
            NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK, "Please fill out all fields.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        g_free(message);
        return;
    }

    // Build macOS Mail command
    char cmd[4096];
    char encoded_message[4096];

    // Replace newline characters with %0A for URL encoding
    int j = 0;
    for (int i = 0; message[i] != '\0' && j < sizeof(encoded_message) - 4; i++) {
        if (message[i] == '\n') {
            encoded_message[j++] = '%';
            encoded_message[j++] = '0';
            encoded_message[j++] = 'A';
        } else {
            encoded_message[j++] = message[i];
        }
    }
    encoded_message[j] = '\0';

    snprintf(cmd, sizeof(cmd),
             "open \"mailto:%s?subject=Report%%20from%%20%s&body=Name:%%20%s%%0AEmail:%%20%s%%0A%%0AMessage:%%0A%s\"",
             "RafaelMercadoEspinoza@gmail.com",
             name,
             name,
             email,
             encoded_message);

    system(cmd);
    g_free(message);

    GtkWidget *dialog = gtk_message_dialog_new(
        NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO,
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

    // Name field
    GtkWidget *entry_name = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_name), "Your Name");
    gtk_box_pack_start(GTK_BOX(vbox), entry_name, FALSE, FALSE, 0);

    // Email field
    GtkWidget *entry_email = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_email), "Your Email");
    gtk_box_pack_start(GTK_BOX(vbox), entry_email, FALSE, FALSE, 0);

    // Message field
    GtkWidget *text_message = gtk_text_view_new();
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_message), GTK_WRAP_WORD);
    gtk_box_pack_start(GTK_BOX(vbox), text_message, TRUE, TRUE, 0);

    GtkWidget *btn_submit = gtk_button_new_with_label("Submit Report");

    GtkWidget *entries[3] = { entry_name, entry_email, text_message };
    g_signal_connect(btn_submit, "clicked", G_CALLBACK(on_submit_clicked), entries);
    gtk_box_pack_start(GTK_BOX(vbox), btn_submit, FALSE, FALSE, 0);

    gtk_widget_show_all(window);
    return window;
}