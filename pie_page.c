#include <stdio.h>
#include <glib/gi18n.h>
#include<gtk/gtk.h>

#include "base_defs.h"
#include "indent_print.h"

#include "custom_pie_widget.h"

static void cancel_clicked ( GtkButton *button,  gpointer   user_data) {
  // ALWAYS called from home_page, so go back to it !
  phdl_grp all_hdls = (phdl_grp)user_data;

  gtk_container_remove ( GTK_CONTAINER ( all_hdls->parentWin ) , all_hdls->vbox_chart_page );
  gtk_widget_destroy ( all_hdls->vbx_hdls->cp_myPie );
  all_hdls->vbx_hdls->cp_myPie = NULL;

  // Go back to home page screen
  gtk_container_add ( GTK_CONTAINER ( all_hdls->parentWin ) , all_hdls->vbox_home_page );
  gtk_widget_show_all ( all_hdls->parentWin );
}

int calc_stats( phdl_grp pall_hdls ) {
  int rc=0;

  float income=0.0, housing=0.0, util = 0.0, food=0.0, transport=0.0, entertain=0.0, health=0.0, work=0.0, other=0.0;
  float total=0.0;

  printf( "  >> E %s L%4d   , pall_hdls=%p \n" , __FUNCTION__ , __LINE__, pall_hdls );

  if ( pall_hdls->vbox_chart_page != NULL ) {
    if ( pall_hdls->t_lst != NULL ) {
      float inc_per, hou_per, util_per ,foo_per, tpr_per , ent_per ,hlt_per , wrk_per, otr_per;
      pokane_grp list = pall_hdls->t_lst;

      while ( list ) {
        int mult = 1;
        float amt;
        if ( list->amount < 0 ) mult = -1;
        amt = mult * list->amount;

        switch ( list->category ) {
          case INCOME_ID:
            income += amt;
            break;
          case HOUSING_ID:
            housing += amt;
            break;
          case UTIL_ID:
            util += amt;
            break;
          case FOOD_ID:
            food += amt;
            break;
          case TRANSPORT_ID:
            transport += amt;
            break;
          case ENTERTAIN_ID:
            entertain += amt;
            break;
          case HEALTH_ID:
            health += amt;
            break;
          case WORK_ID:
            work += amt;
            break;
          default:
            /* other */
            other += amt;
            break;
        }
        total += amt;

        list = list->next;
      }  // while

      inc_per  = income/total;

      if ( housing < 0 ) housing *= -1;
      if ( food < 0 )    food    *= -1;
      if ( util < 0 )    util    *= -1;
      if ( transport < 0 )    transport    *= -1;
      if ( entertain < 0 )    entertain    *= -1;
      if ( health    < 0 )    entertain    *= -1;
      if ( work      < 0 )    work         *= -1;
      if ( other     < 0 )    other        *= -1;

      hou_per  = housing/total;
      foo_per  = food/total;
      util_per = util/total;
      tpr_per  = transport/total;
      ent_per  = entertain/total;
      hlt_per  = health/total;
      wrk_per  = work/total;
      otr_per  = other/total;

      printf( "\n\n  inc = %4.2f hou = %4.2f foo = %4.2f util = %4.2f tpr = %4.2f ent = %4.2f hlth = %4.2f wrk = %4.2f otr = %4.2f \n" ,
                                              income, housing, food, util, transport, entertain, health, work, other );

      printf( "\n\n  inc_per = %4.2f hou_per = %4.2f foo_per = %4.2f tpr= %4.2f ent = %4.2f hlth = %4.2f wrk = %4.2f otr = %4.2f \n" ,
                                              inc_per , hou_per , foo_per, tpr_per, ent_per, hlt_per , wrk_per , otr_per );

      if ( foo_per < 0.0225 ) {
        other += food;
        food  = 0.0;
      }
      if ( util_per < 0.025 ) {
        other += util;
        util  = 0.0;
      }
      tpr_per = transport/total;
      if ( tpr_per < 0.025 ) {
        other += transport;
        transport  = 0.0;
      }
      ent_per = entertain/total;
      if ( ent_per < 0.025 ) {
        other += entertain;
        entertain  = 0.0;
      }
      hlt_per = health/total;
      if ( hlt_per < 0.025 ) {
        other += health;
        health  = 0.0;
      }
      wrk_per = work/total;
      if ( wrk_per < 0.025 ) {
        other += work;
        work  = 0.0;
      }

      otr_per = other/total;

      printf( "  inc_per = %4.2f hou_per = %4.2f foo_per = %4.2f tpr= %4.2f ent = %4.2f hlth = %4.2f wrk = %4.2f Other = %4.2f \n\n" ,
                                              inc_per , hou_per , foo_per, tpr_per, ent_per, hlt_per , wrk_per , otr_per );


      pie_widget_add_slice_to_pie ( (PieWidget *) pall_hdls->vbx_hdls->cp_myPie , inc_per, "#20f020", "Income");
      pie_widget_add_slice_to_pie ( (PieWidget *) pall_hdls->vbx_hdls->cp_myPie , hou_per, "#FF6484", "Housing");
#ifdef OLD_COLORS
      if ( util_per > 0.0 )    pie_widget_add_slice_to_pie ( (PieWidget *) pall_hdls->vbx_hdls->cp_myPie , foo_per, "#36A282", "Util");
      if ( foo_per > 0.0 )     pie_widget_add_slice_to_pie ( (PieWidget *) pall_hdls->vbx_hdls->cp_myPie , foo_per, "#FFC686", "Food");

      if ( tpr_per > 0.0 ) pie_widget_add_slice_to_pie ( (PieWidget *) pall_hdls->vbx_hdls->cp_myPie , tpr_per, "#f00000", "Tport");
      if ( ent_per > 0.0 ) pie_widget_add_slice_to_pie ( (PieWidget *) pall_hdls->vbx_hdls->cp_myPie , ent_per, "#1010A0", "Entertainment");
      if ( hlt_per > 0.0 ) pie_widget_add_slice_to_pie ( (PieWidget *) pall_hdls->vbx_hdls->cp_myPie , hlt_per, "#10A0A0", "Health");
      if ( wrk_per > 0.0 ) pie_widget_add_slice_to_pie ( (PieWidget *) pall_hdls->vbx_hdls->cp_myPie , wrk_per, "#A010A0", "Work");

      pie_widget_add_slice_to_pie ( (PieWidget *) pall_hdls->vbx_hdls->cp_myPie , otr_per, "#1010A0", "Other");
#else
      if ( util_per > 0.025 )    pie_widget_add_slice_to_pie ( (PieWidget *) pall_hdls->vbx_hdls->cp_myPie , foo_per, "#f0f080", "Util");
      if ( foo_per  > 0.025 )    pie_widget_add_slice_to_pie ( (PieWidget *) pall_hdls->vbx_hdls->cp_myPie , foo_per, "#80f5f5", "Food");
      if ( tpr_per  > 0.025 )    pie_widget_add_slice_to_pie ( (PieWidget *) pall_hdls->vbx_hdls->cp_myPie , tpr_per, "#fff0ac", "Tport");
      if ( ent_per  > 0.025 )    pie_widget_add_slice_to_pie ( (PieWidget *) pall_hdls->vbx_hdls->cp_myPie , ent_per, "#ffa0a6", "Entertainment");

      if ( hlt_per > 0.025 )     pie_widget_add_slice_to_pie ( (PieWidget *) pall_hdls->vbx_hdls->cp_myPie , hlt_per, "#10A0A0", "Health");
      if ( wrk_per > 0.025 )     pie_widget_add_slice_to_pie ( (PieWidget *) pall_hdls->vbx_hdls->cp_myPie , wrk_per, "#A010A0", "Work");

      pie_widget_add_slice_to_pie ( (PieWidget *) pall_hdls->vbx_hdls->cp_myPie , otr_per, "#303090", "Other");
#endif
    }  // t_lst != NULL
  }
  else {
    printf( "    pall_hdls->vbox_chart_page = %p \n" , pall_hdls->vbox_chart_page );
  }

  printf( "  << Lv %s L%4d   , pall_hdls=%p \n" , __FUNCTION__ , __LINE__, pall_hdls );

  return rc;
}

int create_pie_chart_page( phdl_grp pall_hdls ) {

  int rc = 0;

  if ( pall_hdls != NULL ) {
    if ( pall_hdls->flg->dbg ) {
      LOG_BLOCK_START ( "  >> E %s \n" , __FUNCTION__ );
      LOG_INDENTED ( "    flgs->dbg = %d \n" , pall_hdls->flg->dbg );
      LOG_INDENTED ( "    parentWin = %p \n" , pall_hdls->parentWin );
    }
  }
  else {
     LOG_BLOCK_START ( "  >> E %s  , pall_hdls = NULL !!! \n" , __FUNCTION__ );
  }

  if ( pall_hdls->vbox_chart_page == NULL ) {
    GtkWidget *piMy;
    GtkWidget *hbox , *button;
    GtkWidget *title_lbl;

    pall_hdls->vbox_chart_page = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);

    LOG_INDENTED ( "    vbox = %p  , %s L%4d \n" , pall_hdls->vbox_chart_page , __FILE__ , __LINE__  );

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
    gtk_box_pack_start( GTK_BOX( pall_hdls->vbox_chart_page ), piMy, TRUE, TRUE, 0);

    hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_pack_end (GTK_BOX ( pall_hdls->vbox_chart_page ), hbox, FALSE, FALSE, 0);

    button = gtk_button_new_with_mnemonic ("_Done");
    pall_hdls->vbx_hdls->cp_pie_done_btn = button;

    g_signal_connect (button, "clicked", G_CALLBACK ( cancel_clicked ), (gpointer) pall_hdls  );

    gtk_box_pack_start (GTK_BOX (hbox), button, FALSE, FALSE, 0);
    calc_stats( pall_hdls );

		// add ref to widget so it doesn't get destroyed on container_remove !
    g_object_ref ( pall_hdls->vbox_chart_page );

  }
 else {
    gtk_container_add (GTK_CONTAINER ( pall_hdls->parentWin ), pall_hdls->vbox_chart_page );
  }

  if ( pall_hdls->vbx_hdls->cp_myPie == NULL ) {
    // Do the graph

    pall_hdls->vbx_hdls->cp_myPie = pie_widget_new();

    gtk_box_pack_start( GTK_BOX( pall_hdls->vbox_chart_page ), pall_hdls->vbx_hdls->cp_myPie,  FALSE, TRUE, 0);
    pie_widget_set_text( (PieWidget *)pall_hdls->vbx_hdls->cp_myPie , GLG_TITLE_T , "" );

    calc_stats( pall_hdls );
  }

 gtk_widget_show_all ( pall_hdls->vbox_chart_page );

  if ( pall_hdls->flg->dbg ) LOG_BLOCK_END ( "  Lv  %s pall_hdls =%p \n" , __FUNCTION__ , pall_hdls );

  return rc;
} // create_pie_chart_page

int create_pie_chart_page_rtn( phdl_grp *all_hdls ) {
  int rc = 0;
  phdl_grp pall_hdls = *all_hdls;

  if ( pall_hdls->flg->dbg ) LOG_BLOCK_START ( "  E  %s  ,  *all_hdls = %p pall_hdls =%p \n" , __FUNCTION__ , *all_hdls , pall_hdls );
  rc = create_pie_chart_page( pall_hdls );

  if ( pall_hdls != NULL ) {
    LOG_INDENTED ( "      pall_hdls->vbox_transact_page = %p \n" , pall_hdls->vbox_transact_page );
  }
  *all_hdls = pall_hdls;

  if ( pall_hdls->flg->dbg ) LOG_BLOCK_END ( "  Lv %s  *all_hdls = %p pall_hdls =%p \n" , __FUNCTION__ , *all_hdls , pall_hdls );
  return rc;
}
