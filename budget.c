#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "base_defs.h"
#include "indent_print.h"
#include "budget.h"
#include "home_page.h"

#define BUDGET_MIN   0
#define BUDGET_MAX   10000
#define BUDGET_STEP  100

static GtkWidget *g_lbl_budget = NULL;
static GtkWidget *g_entry_budget = NULL;
static GtkWidget *g_scale_budget = NULL;

//static app_flags flgs;

static void update_budget_label(phdl_grp pall_hdls) {
  if (!g_lbl_budget) return;
  char buf[128];
  snprintf(buf, sizeof(buf), "Current Budget: %d", pall_hdls->budget_val_tmp);
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

static void adopt_budget_value(long new_value, gboolean sync_entry, gboolean sync_scale, phdl_grp pall_hdls) {

  if ( pall_hdls->flg->dbg ) LOG_BLOCK_START ( "  >> E %s L%4d \n" , __func__, __LINE__ );

  if (new_value < BUDGET_MIN) new_value = BUDGET_MIN;
  if (new_value > BUDGET_MAX) new_value = BUDGET_MAX;
  if (BUDGET_STEP > 1) {
      long r = new_value % BUDGET_STEP;
      if (r >= BUDGET_STEP / 2) new_value += (BUDGET_STEP - r);
      else new_value -= r;
  }

  pall_hdls->budget_val_tmp = new_value;

  update_budget_label( pall_hdls );

  if (sync_entry) set_entry_text_from_value( pall_hdls->budget_val_tmp );
  if (sync_scale) set_scale_from_value( pall_hdls->budget_val_tmp );
}

static void hide_budget_page( phdl_grp all_hdls ) {
  gtk_widget_hide( all_hdls->vbox_budget_page );

  gtk_container_remove ( GTK_CONTAINER ( all_hdls->parentWin ) , all_hdls->vbox_budget_page );
}

static void on_budget_back_clicked_cb(GtkButton *btn, gpointer user_data) {
  phdl_grp pall_hdls = (phdl_grp)user_data;

  hide_budget_page ( pall_hdls );

  gtk_container_add  (GTK_CONTAINER(pall_hdls->parentWin), pall_hdls->vbox_home_page);
  gtk_widget_show_all(pall_hdls->parentWin);
}

static void on_save_clicked(GtkButton *btn, gpointer data) {
  phdl_grp pall_hdls = (phdl_grp)data;

  pall_hdls->budget_val = pall_hdls->budget_val_tmp;

  strcpy( pall_hdls->budget_str,  g_strdup_printf( "%d", pall_hdls->budget_val ) );

  home_update_budget_display(pall_hdls);

  update_budget_label(pall_hdls);

  hide_budget_page ( pall_hdls );

  gtk_container_add  (GTK_CONTAINER( pall_hdls->parentWin), pall_hdls->vbox_home_page );
  gtk_widget_show_all(pall_hdls->parentWin);

  g_print("[Budget] SAVE clicked: %d (add DB save later)\n", pall_hdls->budget_val);
}

static void on_scale_value_changed(GtkRange *range, gpointer user_data) {
  phdl_grp pall_hdls = (phdl_grp)user_data;
  long val = (long) gtk_range_get_value(range);

  adopt_budget_value( val, TRUE, FALSE, pall_hdls );
}

static void on_entry_activate(GtkEntry *entry, gpointer user_data) {
  phdl_grp pall_hdls = (phdl_grp)user_data;
  const gchar *text = gtk_entry_get_text(entry);
  char *endp = NULL;
  long val = strtol(text, &endp, 10);

  if (text == endp) {
    set_entry_text_from_value(pall_hdls->budget_val);
    return;
  }
  adopt_budget_value( val, FALSE, TRUE, pall_hdls );
}

static void on_entry_changed(GtkEditable *editable, gpointer user_data) {
  const gchar *text = gtk_entry_get_text(GTK_ENTRY(editable));
  char *endp = NULL;
  long val = strtol(text, &endp, 10);
  phdl_grp pall_hdls = (phdl_grp)user_data;

  if ( pall_hdls->flg->dbg ) LOG_BLOCK_START ( "  >> E %s L%4d \n" , __func__, __LINE__ );

  if (text != endp) {
    if (val < BUDGET_MIN) val = BUDGET_MIN;
    if (val > BUDGET_MAX) val = BUDGET_MAX;
    pall_hdls->budget_val = val;
    update_budget_label( pall_hdls );
  }

  if ( pall_hdls->flg->dbg ) LOG_BLOCK_END ( "  >> Lv %s L%4d \n" , __func__, __LINE__ );
}

int create_budget_page(phdl_grp pall_hdls) {
  int rc  = 0;
  //pall_hdls->flg = &flgs;

pall_hdls->flg->dbg = 1;

  if ( pall_hdls->flg->dbg ) LOG_BLOCK_START ( "  >> E %s L%4d \n" , __func__, __LINE__ );

  if (!pall_hdls) return -1;

  // If pointer exists but widget got finalized somehow, rebuild
  if (pall_hdls->vbox_budget_page && !GTK_IS_WIDGET(pall_hdls->vbox_budget_page)) {
    pall_hdls->vbox_budget_page = NULL;
  }

  if (pall_hdls->vbox_budget_page == NULL) {

    GtkWidget *label, *label2;
    GtkWidget *vbox;
    GtkWidget *vbox_budget;
    GtkWidget *hbox_budget;

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_container_set_border_width(GTK_CONTAINER( vbox ), 16);
    pall_hdls->vbox_budget_page = vbox;

    // Keep the vbox alive when removed from parent (prevents blank on 2nd open)
    g_object_ref(pall_hdls->vbox_budget_page);

    // Label
    g_lbl_budget = gtk_label_new(NULL);
    gtk_widget_set_halign(g_lbl_budget, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX( vbox ), g_lbl_budget, FALSE, FALSE, 0);

    // Frame + content
    GtkWidget *frame_budget = gtk_frame_new("Set Budget");
    gtk_box_pack_start(GTK_BOX( vbox ), frame_budget, FALSE, FALSE, 0);

    vbox_budget = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    hbox_budget = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

    gtk_container_set_border_width(GTK_CONTAINER( vbox_budget), 10);
    gtk_container_add(GTK_CONTAINER(frame_budget), vbox_budget);

    // Slider
     GtkAdjustment *adj = gtk_adjustment_new(
       (gdouble)pall_hdls->budget_val_tmp,
       (gdouble)BUDGET_MIN,
       (gdouble)BUDGET_MAX,
       (gdouble)BUDGET_STEP,
       (gdouble)(BUDGET_STEP * 10),0.0
     );
     g_scale_budget = gtk_scale_new(GTK_ORIENTATION_HORIZONTAL, adj);
     gtk_scale_set_draw_value(GTK_SCALE(g_scale_budget), TRUE);
     gtk_box_pack_start(GTK_BOX( vbox_budget), g_scale_budget, FALSE, FALSE, 0);

     gtk_box_pack_start(GTK_BOX( vbox_budget), hbox_budget , FALSE, FALSE, 0);

    // Entry
    g_entry_budget = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(g_entry_budget), "Enter budget (e.g., 2500)");
    label = gtk_label_new_with_mnemonic ("     ");
    label2 = gtk_label_new_with_mnemonic ("     ");
    gtk_box_pack_start(GTK_BOX( hbox_budget), g_entry_budget, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX( hbox_budget), label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX( hbox_budget), label2, FALSE, FALSE, 0);

     // Buttons
     GtkWidget *btn_row  = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
     GtkWidget *btn_save = gtk_button_new_with_label("Save");
     GtkWidget *btn_back = gtk_button_new_with_label("Back");
     gtk_box_pack_start(GTK_BOX(btn_row), btn_save, FALSE, FALSE, 0);
     gtk_box_pack_end  (GTK_BOX(btn_row), btn_back, FALSE, FALSE, 0);
     gtk_box_pack_start(GTK_BOX( vbox ),   btn_row,  FALSE, FALSE, 0);

     // Signals
     g_signal_connect(g_scale_budget, "value-changed", G_CALLBACK(on_scale_value_changed), pall_hdls);
     g_signal_connect(g_entry_budget, "activate",      G_CALLBACK(on_entry_activate), pall_hdls);
     g_signal_connect(g_entry_budget, "changed",       G_CALLBACK(on_entry_changed),  pall_hdls);
     g_signal_connect(btn_save, "clicked",             G_CALLBACK(on_save_clicked),    pall_hdls);
     g_signal_connect(btn_back, "clicked",             G_CALLBACK(on_budget_back_clicked_cb), pall_hdls);

     // Initial sync
     pall_hdls->budget_val_tmp = pall_hdls->budget_val;
     adopt_budget_value(pall_hdls->budget_val, TRUE, TRUE, pall_hdls);
  }
  else {
     gtk_container_add (GTK_CONTAINER ( pall_hdls->parentWin ), pall_hdls->vbox_budget_page );
     pall_hdls->vbox_active = pall_hdls->vbox_budget_page;
     gtk_widget_show_all ( pall_hdls->vbox_budget_page );
  }

  if ( pall_hdls->flg->dbg ) LOG_BLOCK_END ( "  << Lv %s  L%4d \n" , __func__, __LINE__ );

  return rc;
} //create_budget_page

int create_budget_page_rtn(phdl_grp *all_hdls) {
  int rc  = 0;
  phdl_grp pall_hdls = *all_hdls;
  //pall_hdls->flg = &flgs;

  if ( pall_hdls->flg->dbg ) LOG_BLOCK_START ( "  >> E %s L%4d \n" , __func__, __LINE__ );

  create_budget_page( pall_hdls );

  *all_hdls = pall_hdls;

  if ( pall_hdls->flg->dbg ) LOG_BLOCK_END ( "  << Lv %s L%4d \n" , __func__, __LINE__ );
  return rc;
}
