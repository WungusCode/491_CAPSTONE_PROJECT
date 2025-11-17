#include <gtk/gtk.h>
#include <glib.h>

#include "../base_defs.h"
#include "../budget.h"
#include "../home_page.h"

extern GtkWidget *g_lbl_budget;
extern GtkWidget *g_entry_budget;
extern GtkWidget *g_scale_budget;

typedef struct {
    hdl_grp   all_hdls;
    phdl_grp  pall_hdls;
    app_flags flgs;
} BudgetTestFixture;


static void setup_fixture(BudgetTestFixture *fixture, gconstpointer user_data) {
    fixture->pall_hdls = &fixture->all_hdls;
    memset(&fixture->all_hdls, 0, sizeof(hdl_grp));
    
    fixture->pall_hdls->flg = &fixture->flgs;
    fixture->pall_hdls->flg->dbg = 1;

    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    fixture->pall_hdls->parentWin = window;

    fixture->pall_hdls->budget_val = 1000;
    strcpy( fixture->pall_hdls->budget_str, "1000" );
    
    fixture->pall_hdls->vbox_home_page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    g_object_ref(fixture->pall_hdls->vbox_home_page);
    
    fixture->pall_hdls->vbox_budget_page = NULL;
    
    create_budget_page(fixture->pall_hdls);
    
    gtk_container_add(GTK_CONTAINER(window), fixture->pall_hdls->vbox_budget_page);
    gtk_widget_show_all(window);
}

static void teardown_fixture(BudgetTestFixture *fixture, gconstpointer user_data) {
    if (fixture->pall_hdls->parentWin) {
        gtk_widget_destroy(fixture->pall_hdls->parentWin);
    }
}

static void run_gtk_events(void) {
    while (gtk_events_pending()) {
        gtk_main_iteration();
    }
}

// --- Test Cases ---
static void test_scale_updates_entry(BudgetTestFixture *fixture, gconstpointer user_data) {
    gtk_range_set_value(GTK_RANGE(g_scale_budget), 2500);
    
    run_gtk_events();

    const char *entry_text = gtk_entry_get_text(GTK_ENTRY(g_entry_budget));
    g_assert_cmpstr(entry_text, ==, "2500");
}

static void test_entry_updates_scale(BudgetTestFixture *fixture, gconstpointer user_data) {
    gtk_entry_set_text(GTK_ENTRY(g_entry_budget), "7000");
    g_signal_emit_by_name(g_entry_budget, "activate", NULL);
    
    run_gtk_events();
    
    double scale_val = gtk_range_get_value(GTK_RANGE(g_scale_budget));
    g_assert_cmpfloat(scale_val, ==, 7000.0);
}

static void test_save_updates_model(BudgetTestFixture *fixture, gconstpointer user_data) {
    phdl_grp pall_hdls = fixture->pall_hdls;
    
    gtk_range_set_value(GTK_RANGE(g_scale_budget), 1200);
    run_gtk_events();

    GList *vbox_children = gtk_container_get_children(GTK_CONTAINER(pall_hdls->vbox_budget_page));
    GtkWidget *btn_row = g_list_last(vbox_children)->data;
    GList *btn_row_children = gtk_container_get_children(GTK_CONTAINER(btn_row));
    GtkWidget *save_button = btn_row_children->data;
    
    g_signal_emit_by_name(save_button, "clicked", NULL);
    g_list_free(vbox_children);
    g_list_free(btn_row_children);
    
    run_gtk_events();
    
    g_assert_cmpint(pall_hdls->budget_val, ==, 1200);
}

int main(int argc, char **argv) {
    g_test_init(&argc, &argv, NULL);
    
    gtk_init(&argc, &argv);

    BudgetTestFixture fixture;
    setup_fixture(&fixture, NULL);

    g_test_add_data_func("/budget/scale_updates_entry", &fixture, test_scale_updates_entry);
    g_test_add_data_func("/budget/entry_updates_scale", &fixture, test_entry_updates_scale);
    g_test_add_data_func("/budget/save_updates_model", &fixture, test_save_updates_model);

    int result = g_test_run();
    
    teardown_fixture(&fixture, NULL);

    return result;
}