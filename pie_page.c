#include <stdio.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <math.h>

#include "base_defs.h"
#include "indent_print.h"
#include "custom_pie_widget.h"

static void cancel_clicked (GtkButton *button, gpointer user_data) {
  (void)button;
  phdl_grp all_hdls = (phdl_grp)user_data;

  gtk_container_remove(GTK_CONTAINER(all_hdls->parentWin), all_hdls->vbox_chart_page);
  if (all_hdls->vbx_hdls->cp_myPie) {
    gtk_widget_destroy(all_hdls->vbx_hdls->cp_myPie);
    all_hdls->vbx_hdls->cp_myPie = NULL;
  }

  gtk_container_add(GTK_CONTAINER(all_hdls->parentWin), all_hdls->vbox_home_page);
  gtk_widget_show_all(all_hdls->parentWin);
}

/* ---------- Centered two-column percentage list under the pie ---------- */
static GtkWidget* build_percent_list_widget(int inc_pct,
                                            int rent_pct,
                                            int food_pct,
                                            int tpt_pct,
                                            int ent_pct,
                                            int oth_pct)
{
  GtkWidget *outer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
  gtk_widget_set_halign(outer, GTK_ALIGN_CENTER);

  GtkWidget *title = gtk_label_new(NULL);
  gtk_label_set_markup(GTK_LABEL(title), "<b>Amount per Category Spent</b>");
  gtk_widget_set_halign(title, GTK_ALIGN_CENTER);
  gtk_box_pack_start(GTK_BOX(outer), title, FALSE, FALSE, 0);

  GtkWidget *grid = gtk_grid_new();
  gtk_grid_set_row_spacing(GTK_GRID(grid), 6);
  gtk_grid_set_column_spacing(GTK_GRID(grid), 24);
  gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
  gtk_box_pack_start(GTK_BOX(outer), grid, FALSE, FALSE, 0);

  struct Row { const char *name; int pct; } rows[] = {
    {"Income",        inc_pct},
    {"Rent",          rent_pct},
    {"Food",          food_pct},
    {"Transport",     tpt_pct},
    {"Entertainment", ent_pct},
    {"Others",        oth_pct},
  };

  for (int r = 0; r < (int)(sizeof(rows)/sizeof(rows[0])); ++r) {
    GtkWidget *ln = gtk_label_new(rows[r].name);
    gtk_widget_set_halign(ln, GTK_ALIGN_START);
    GtkWidget *lp = gtk_label_new(NULL);
    char buf[16]; g_snprintf(buf, sizeof(buf), "%d%%", rows[r].pct);
    gtk_label_set_text(GTK_LABEL(lp), buf);
    gtk_widget_set_halign(lp, GTK_ALIGN_END);
    gtk_grid_attach(GTK_GRID(grid), ln, 0, r, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), lp, 1, r, 1, 1);
  }

  gtk_widget_set_name(outer, "pct_list_block");
  return outer;
}

static void add_or_replace_percent_list(GtkWidget *vbox, GtkWidget *list_block)
{
  GList *children = gtk_container_get_children(GTK_CONTAINER(vbox));
  for (GList *l = children; l != NULL; l = l->next) {
    GtkWidget *w = GTK_WIDGET(l->data);
    const char *nm = gtk_widget_get_name(w);
    if (nm && g_strcmp0(nm, "pct_list_block") == 0) {
      gtk_container_remove(GTK_CONTAINER(vbox), w);
      break;
    }
  }
  g_list_free(children);

  gtk_box_pack_start(GTK_BOX(vbox), list_block, FALSE, FALSE, 0);
}

/* ---------- Compute totals, add pie slices, then render the centered list ---------- */
int calc_stats(phdl_grp pall_hdls)
{
  int rc = 0;

  float income = 0.0f, housing = 0.0f, util = 0.0f, food = 0.0f, transport = 0.0f,
        entertain = 0.0f, health = 0.0f, work = 0.0f, other = 0.0f;
  float total = 0.0f;

  printf("  >> E %s   pall_hdls=%p \n", __FUNCTION__, pall_hdls);

  if (pall_hdls->vbox_chart_page != NULL) {
    if (pall_hdls->t_lst != NULL) {
      /* First pass: accumulate absolute amounts by category */
      pokane_grp list = pall_hdls->t_lst;
      while (list) {
        float amt = (list->amount < 0) ? -list->amount : list->amount;

        switch (list->category) {
          case INCOME_ID:     income    += amt; break;
          case HOUSING_ID:    housing   += amt; break;
          case UTIL_ID:       util      += amt; break;
          case FOOD_ID:       food      += amt; break;
          case TRANSPORT_ID:  transport += amt; break;
          case ENTERTAIN_ID:  entertain += amt; break;
          case HEALTH_ID:     health    += amt; break;
          case WORK_ID:       work      += amt; break;
          default:            other     += amt; break;
        }
        total += amt;
        list = list->next;
      }

      /* Guard divide-by-zero */
      if (total <= 0.0f) {
        total = 1.0f;
      }

      /* Initial percentages */
      float inc_per  = income    / total;
      float hou_per  = housing   / total;
      float util_per = util      / total;
      float foo_per  = food      / total;
      float tpr_per  = transport / total;
      float ent_per  = entertain / total;
      float hlt_per  = health    / total;
      float wrk_per  = work      / total;
      float otr_per  = other     / total;

      /* Merge tiny slices into Others (thresholds ~2.5%) */
      if (foo_per  < 0.0250f) { other += food;      food = 0.0f; }
      if (util_per < 0.0250f) { other += util;      util = 0.0f; }
      if (tpr_per  < 0.0250f) { other += transport; transport = 0.0f; }
      if (ent_per  < 0.0250f) { other += entertain; entertain = 0.0f; }
      if (hlt_per  < 0.0250f) { other += health;    health = 0.0f; }
      if (wrk_per  < 0.0250f) { other += work;      work = 0.0f; }

      /* Recompute percentages AFTER merging so pie and list match */
      inc_per  = income    / total;
      hou_per  = housing   / total;
      util_per = util      / total;
      foo_per  = food      / total;
      tpr_per  = transport / total;
      ent_per  = entertain / total;
      hlt_per  = health    / total;
      wrk_per  = work      / total;
      otr_per  = other     / total;

      /* Pie slices */
      pie_widget_add_slice_to_pie((PieWidget*)pall_hdls->vbx_hdls->cp_myPie, inc_per, "#20f020", "Income");
      if (hou_per  > 0.0f)
        pie_widget_add_slice_to_pie((PieWidget*)pall_hdls->vbx_hdls->cp_myPie, hou_per,  "#FF6484", "Housing");

#ifdef OLD_COLORS
      if (util_per > 0.0f)
        pie_widget_add_slice_to_pie((PieWidget*)pall_hdls->vbx_hdls->cp_myPie, util_per, "#36A282", "Util");
      if (foo_per  > 0.0f)
        pie_widget_add_slice_to_pie((PieWidget*)pall_hdls->vbx_hdls->cp_myPie, foo_per,  "#FFC686", "Food");
      if (tpr_per  > 0.0f)
        pie_widget_add_slice_to_pie((PieWidget*)pall_hdls->vbx_hdls->cp_myPie, tpr_per,  "#3399FF", "Tport");   /* new color */
      if (ent_per  > 0.0f)
        pie_widget_add_slice_to_pie((PieWidget*)pall_hdls->vbx_hdls->cp_myPie, ent_per,  "#1010A0", "Entertainment");
      if (hlt_per  > 0.0f)
        pie_widget_add_slice_to_pie((PieWidget*)pall_hdls->vbx_hdls->cp_myPie, hlt_per,  "#10A0A0", "Health");
      if (wrk_per  > 0.0f)
        pie_widget_add_slice_to_pie((PieWidget*)pall_hdls->vbx_hdls->cp_myPie, wrk_per,  "#A010A0", "Work");
      if (otr_per  > 0.0f)
        pie_widget_add_slice_to_pie((PieWidget*)pall_hdls->vbx_hdls->cp_myPie, otr_per,  "#1010A0", "Other");
#else
      if (util_per > 0.025f)
        pie_widget_add_slice_to_pie((PieWidget*)pall_hdls->vbx_hdls->cp_myPie, util_per, "#f0f080", "Util");
      if (foo_per  > 0.025f)
        pie_widget_add_slice_to_pie((PieWidget*)pall_hdls->vbx_hdls->cp_myPie, foo_per,  "#80f5f5", "Food");
      if (tpr_per  > 0.025f)
        pie_widget_add_slice_to_pie((PieWidget*)pall_hdls->vbx_hdls->cp_myPie, tpr_per,  "#3399FF", "Tport");   /* new color */
      if (ent_per  > 0.025f)
        pie_widget_add_slice_to_pie((PieWidget*)pall_hdls->vbx_hdls->cp_myPie, ent_per,  "#ffa0a6", "Entertainment");
      if (hlt_per  > 0.025f)
        pie_widget_add_slice_to_pie((PieWidget*)pall_hdls->vbx_hdls->cp_myPie, hlt_per,  "#10A0A0", "Health");
      if (wrk_per  > 0.025f)
        pie_widget_add_slice_to_pie((PieWidget*)pall_hdls->vbx_hdls->cp_myPie, wrk_per,  "#A010A0", "Work");
      if (otr_per  > 0.0f)
        pie_widget_add_slice_to_pie((PieWidget*)pall_hdls->vbx_hdls->cp_myPie, otr_per,  "#303090", "Other");
#endif

      /* Build the centered percentage list from the same (post-merge) percents */
      int inc_pct = (int)lroundf(inc_per * 100.0f);
      int hou_pct = (int)lroundf(hou_per * 100.0f);
      int foo_pct = (int)lroundf(foo_per * 100.0f);
      int tpr_pct = (int)lroundf(tpr_per * 100.0f);
      int ent_pct = (int)lroundf(ent_per * 100.0f);
      int oth_pct = (int)lroundf(otr_per * 100.0f);

      /* Explicit zeroing isn’t strictly needed now that we recompute after merge,
         but it’s harmless and documents intent. */
      if (hou_per <= 0.0f) hou_pct = 0;
      if (foo_per <= 0.0f) foo_pct = 0;
      if (tpr_per <= 0.0f) tpr_pct = 0;
      if (ent_per <= 0.0f) ent_pct = 0;
      if (otr_per <= 0.0f) oth_pct = 0;
      if (inc_per <= 0.0f) inc_pct = 0;  /* include Income, zero if unused */

      GtkWidget *list_block = build_percent_list_widget(
        inc_pct,  /* Income */
        hou_pct,  /* Rent (Housing) */
        foo_pct,  /* Food */
        tpr_pct,  /* Transport */
        ent_pct,  /* Entertainment */
        oth_pct   /* Others */
      );
      add_or_replace_percent_list(pall_hdls->vbox_chart_page, list_block);
    }
  } else {
    printf("    pall_hdls->vbox_chart_page = %p \n", pall_hdls->vbox_chart_page);
  }

  printf("  << Lv %s   pall_hdls=%p \n", __FUNCTION__, pall_hdls);
  return rc;
}

/* ---------- Page constructor: title, pie widget, Done button; slices via calc_stats() ---------- */
int create_pie_chart_page(phdl_grp pall_hdls)
{
  int rc = 0;

  if (pall_hdls && pall_hdls->flg->dbg) {
    LOG_BLOCK_START("  >> E %s \n", __FUNCTION__);
    LOG_INDENTED("    flgs->dbg = %d \n", pall_hdls->flg->dbg);
    LOG_INDENTED("    parentWin = %p \n", pall_hdls->parentWin);
  }

  if (pall_hdls->vbox_chart_page == NULL) {
    GtkWidget *piMy;
    GtkWidget *hbox, *button, *title_lbl;

    pall_hdls->vbox_chart_page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    title_lbl = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title_lbl), "<b>Spending Category</b>");
    gtk_widget_set_halign(title_lbl, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(title_lbl, 8);
    gtk_widget_set_margin_bottom(title_lbl, 8);
    gtk_box_pack_start(GTK_BOX(pall_hdls->vbox_chart_page), title_lbl, FALSE, FALSE, 0);

    piMy = pie_widget_new();
    pall_hdls->vbx_hdls->cp_myPie = piMy;
    pie_widget_set_text((PieWidget*)piMy, GLG_TITLE_T, "");
    gtk_widget_set_size_request(piMy, 520, 420);
    gtk_widget_set_halign(piMy, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(piMy, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(pall_hdls->vbox_chart_page), piMy, TRUE, TRUE, 0);

    /* Bottom row with Done button */
    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_pack_end(GTK_BOX(pall_hdls->vbox_chart_page), hbox, FALSE, FALSE, 0);

    button = gtk_button_new_with_mnemonic("_Done");
    pall_hdls->vbx_hdls->cp_pie_done_btn = button;
    g_signal_connect(button, "clicked", G_CALLBACK(cancel_clicked), pall_hdls);
    gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);

    /* Compute and render slices + centered list */
    calc_stats(pall_hdls);

    /* keep a ref so container_remove doesn't destroy it */
    g_object_ref(pall_hdls->vbox_chart_page);
  } else {
    gtk_container_add(GTK_CONTAINER(pall_hdls->parentWin), pall_hdls->vbox_chart_page);
  }

  if (pall_hdls->vbx_hdls->cp_myPie == NULL) {
    /* Fallback: create pie and compute slices */
    pall_hdls->vbx_hdls->cp_myPie = pie_widget_new();
    gtk_box_pack_start(GTK_BOX(pall_hdls->vbox_chart_page), pall_hdls->vbx_hdls->cp_myPie, FALSE, TRUE, 0);
    pie_widget_set_text((PieWidget*)pall_hdls->vbx_hdls->cp_myPie, GLG_TITLE_T, "");
    calc_stats(pall_hdls);
  }

  gtk_widget_show_all(pall_hdls->vbox_chart_page);

  if (pall_hdls && pall_hdls->flg->dbg) LOG_BLOCK_END("  Lv  %s pall_hdls =%p \n", __FUNCTION__, pall_hdls);
  return rc;
}

int create_pie_chart_page_rtn(phdl_grp *all_hdls)
{
  int rc = 0;
  phdl_grp pall_hdls = *all_hdls;

  if (pall_hdls && pall_hdls->flg->dbg) LOG_BLOCK_START("  E  %s  *all_hdls = %p pall_hdls =%p \n", __FUNCTION__, *all_hdls, pall_hdls);
  rc = create_pie_chart_page(pall_hdls);

  if (pall_hdls) {
    LOG_INDENTED("      pall_hdls->vbox_transact_page = %p \n", pall_hdls->vbox_transact_page);
  }
  *all_hdls = pall_hdls;

  if (pall_hdls && pall_hdls->flg->dbg) LOG_BLOCK_END("  Lv %s  *all_hdls = %p pall_hdls =%p \n", __FUNCTION__, *all_hdls, pall_hdls);
  return rc;
}

