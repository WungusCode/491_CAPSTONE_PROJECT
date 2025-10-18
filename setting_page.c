#include <stdio.h>
#include <gtk/gtk.h>

#include "setting_page.h"
#include "home_page.h"
#include "start_screen.h"
#include "base_defs.h"

// --- Callbacks ---

static void on_close_clicked(GtkButton *btn, gpointer user_data) {
    GtkWidget *window = GTK_WIDGET(user_data);
    if (window) gtk_widget_destroy(window);
}

static void on_notify_toggled(GtkToggleButton *toggle, gpointer user_data) {
    gboolean active = gtk_toggle_button_get_active(toggle);
    g_print("[Settings] Notifications: %s\n", active ? "ON" : "OFF");
}

static void apply_dark_preference(gboolean prefer_dark) {
    GtkSettings *settings = gtk_settings_get_default();
    if (settings) {
        g_object_set(settings, "gtk-application-prefer-dark-theme", prefer_dark, NULL);
        g_print("[Settings] Theme set to %s mode\n", prefer_dark ? "Dark" : "Light");
    } else {
        g_warning("[Settings] Could not get GtkSettings to set theme.");
    }
}

static void on_theme_toggled(GtkToggleButton *toggle, gpointer user_data) {
    // We wire this only to the "Dark" radio; when it becomes active, set dark.
    if (gtk_toggle_button_get_active(toggle)) {
        apply_dark_preference(TRUE);
    }
}

static void on_theme_light_clicked(GtkToggleButton *toggle, gpointer user_data) {
    if (gtk_toggle_button_get_active(toggle)) {
        apply_dark_preference(FALSE);
    }
}
// used to report a problem 
static void on_reportProblem_clicked(GtkButton *btn, gpointer user_data) {
    g_print("[Settings] Report Problem clicked\n");
    // Placeholder for actual report problem functionality
    GtkWidget *report_dialog = gtk_message_dialog_new(GTK_WINDOW(user_data),
                                                      GTK_DIALOG_MODAL,
                                                      GTK_MESSAGE_INFO,
                                                      GTK_BUTTONS_OK,
                                                      "How to report problems here");
    gtk_dialog_run(GTK_DIALOG(report_dialog));
    gtk_widget_destroy(report_dialog);
}

// --- Public API ---

GtkWidget* create_setting_page(void) {
    // New Settings window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Settings");
    gtk_window_set_default_size(GTK_WINDOW(window), WIN_W / 2, WIN_H / 2);

    // Root layout
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 16);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // ===== Notifications =====
    GtkWidget *frame_notify = gtk_frame_new("Notifications");
    GtkWidget *box_notify = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_container_set_border_width(GTK_CONTAINER(box_notify), 8);
    gtk_container_add(GTK_CONTAINER(frame_notify), box_notify);

    // add to main box (only once)
    gtk_box_pack_start(GTK_BOX(vbox), frame_notify, FALSE, FALSE, 0);

    GtkWidget *check_notify = gtk_check_button_new_with_label("Enable Notifications");
    g_signal_connect(check_notify, "toggled", G_CALLBACK(on_notify_toggled), NULL);
    gtk_box_pack_start(GTK_BOX(box_notify), check_notify, FALSE, FALSE, 0);

    // ===== Appearance (Light / Dark) =====
    GtkWidget *frame_theme = gtk_frame_new("Appearance");
    GtkWidget *box_theme = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_container_set_border_width(GTK_CONTAINER(box_theme), 8);
    gtk_container_add(GTK_CONTAINER(frame_theme), box_theme);

    // Radio buttons: Light / Dark
    GtkWidget *radio_light = gtk_radio_button_new_with_label(NULL, "Light mode");
    GtkWidget *radio_dark  = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio_light), "Dark mode");

    g_signal_connect(radio_dark,  "toggled", G_CALLBACK(on_theme_toggled), NULL);
    g_signal_connect(radio_light, "toggled", G_CALLBACK(on_theme_light_clicked), NULL);

    // Initialize radios based on current setting
    GtkSettings *settings = gtk_settings_get_default();
    gboolean prefer_dark = FALSE;
    if (settings) {
        g_object_get(settings, "gtk-application-prefer-dark-theme", &prefer_dark, NULL);
    }
    if (prefer_dark) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio_dark), TRUE);
    } else {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio_light), TRUE);
    }

    gtk_box_pack_start(GTK_BOX(box_theme), radio_light, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_theme), radio_dark,  FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), frame_theme, FALSE, FALSE, 0);

    // ===== Account information (email + password) =====
    GtkWidget *frame_account = gtk_frame_new("Account");
    GtkWidget *box_account = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(box_account), 10);
    gtk_container_add(GTK_CONTAINER(frame_account), box_account);

    // Email entry
    GtkWidget *enter_email = gtk_entry_new();  // was gtk_enter_new()
    gtk_entry_set_placeholder_text(GTK_ENTRY(enter_email), "Email");
    gtk_box_pack_start(GTK_BOX(box_account), enter_email, FALSE, FALSE, 0);

    // Password entry
    GtkWidget *enter_password = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(enter_password), "Password");
    gtk_entry_set_visibility(GTK_ENTRY(enter_password), TRUE);
    gtk_box_pack_start(GTK_BOX(box_account), enter_password, FALSE, FALSE, 0); // missing ';' fixed

    // add to main box
    gtk_box_pack_start(GTK_BOX(vbox), frame_account, FALSE, FALSE, 0);

    // ===== Screen time =====
    GtkWidget *frame_screentime = gtk_frame_new("Screen Time");
    gtk_container_set_border_width(GTK_CONTAINER(frame_screentime), 8); // correct signature
    GtkWidget *label_screentime = gtk_label_new("You have used the app for 2h 15m this week.");
    gtk_container_add(GTK_CONTAINER(frame_screentime), label_screentime);
    gtk_box_pack_start(GTK_BOX(vbox), frame_screentime, FALSE, FALSE, 0);

    // Report a problem button
    GtkWidget *btn_report = gtk_button_new_with_label("Report a problem with the web application");
    g_signal_connect(btn_report, "clicked", G_CALLBACK(on_reportProblem_clicked), window);
    gtk_box_pack_start(GTK_BOX(vbox), btn_report, FALSE, FALSE, 0);

    // Close button at bottom
    GtkWidget *btn_close = gtk_button_new_with_label("Close");
    g_signal_connect(btn_close, "clicked", G_CALLBACK(on_close_clicked), window);
    gtk_box_pack_end(GTK_BOX(vbox), btn_close, FALSE, FALSE, 0);

    // Show
    gtk_widget_show_all(window);

    printf("[Settings] %s\n", __FUNCTION__);
    return window;
}

void destroy_setting_page(GtkWidget *window) {
    if (window) {
        gtk_widget_destroy(window);
    }
    printf("[Settings] window destroyed\n");
}

