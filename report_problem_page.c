#include <curl/curl.h>

struct upload_status {
    int lines_read;
};

static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp) {
    struct upload_status *upload_ctx = (struct upload_status *)userp;
    const char *data = ((const char **)userp)[upload_ctx->lines_read];

    if (!data)
        return 0;

    size_t len = strlen(data);
    memcpy(ptr, data, len);
    upload_ctx->lines_read++;
    return len;
}

static int send_report_email(const char *name, const char *email, const char *message) {
    CURL *curl = curl_easy_init();
    if (!curl) return 1;

    static const char *payload[6];
    char subject[256];
    char body[2048];

    snprintf(subject, sizeof(subject), "Subject: Report from %s\r\n", name);
    snprintf(body, sizeof(body),
             "Name: %s\nEmail: %s\n\nMessage:\n%s\r\n",
             name, email, message);

    payload[0] = "To: RafaelMercadoEspinoza@gmail.com\r\n";
    payload[1] = "From: App Reporter <YOUR_EMAIL@gmail.com>\r\n";
    payload[2] = subject;
    payload[3] = "\r\n";
    payload[4] = body;
    payload[5] = NULL;

    struct upload_status upload_ctx = {0};

    curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.gmail.com:587");
    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);

    curl_easy_setopt(curl, CURLOPT_USERNAME, "YOUR_EMAIL@gmail.com");
    curl_easy_setopt(curl, CURLOPT_PASSWORD, "bqyh mqmd tdsq bnya");

    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, "<YOUR_EMAIL@gmail.com>");

    struct curl_slist *recipients = NULL;
    recipients = curl_slist_append(recipients, "<RafaelMercadoEspinoza@gmail.com>");
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

    curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
    curl_easy_setopt(curl, CURLOPT_READDATA, payload);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

    CURLcode res = curl_easy_perform(curl);

    curl_slist_free_all(recipients);
    curl_easy_cleanup(curl);
    return (int)res;
}
#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include "report_problem_page.h"

static void on_submit_clicked(GtkButton *btn, gpointer user_data) {
    GtkWidget **entries = (GtkWidget **)user_data;

    if (!entries || !entries[0] || !entries[1] || !entries[2]) {
        g_printerr("ERROR: Invalid widget pointers passed to callback.\n");
        return;
    }

    const char *name = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const char *email = gtk_entry_get_text(GTK_ENTRY(entries[1]));

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(entries[2]));
    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);
    char *message = gtk_text_buffer_get_text(buffer, &start, &end, TRUE);

    // NULL safety
    if (!name) name = "";
    if (!email) email = "";
    if (!message) message = "";

    if (strlen(name) == 0 || strlen(email) == 0 || strlen(message) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(
            NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK, "Please fill out all fields.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        g_free(message);
        return;
    }

    int result = send_report_email(name, email, message);
    if (result != 0) {
        GtkWidget *dialog_err = gtk_message_dialog_new(
            NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK, "Failed to send email report.");
        gtk_dialog_run(GTK_DIALOG(dialog_err));
        gtk_widget_destroy(dialog_err);
        g_free(message);
        return;
    }

    GtkWidget *dialog = gtk_message_dialog_new(
        NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK, "Problem report sent!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    g_free(message);
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

    // FIX: allocate entries array on heap so it survives after function returns
    GtkWidget **entries = g_malloc(sizeof(GtkWidget*) * 3);
    entries[0] = entry_name;
    entries[1] = entry_email;
    entries[2] = text_message;

    g_signal_connect(btn_submit, "clicked",
                     G_CALLBACK(on_submit_clicked),
                     entries);

    gtk_box_pack_start(GTK_BOX(vbox), btn_submit, FALSE, FALSE, 0);

    gtk_widget_show_all(window);
    return window;
}
