#include <stdio.h>
#include <glib/gi18n.h>
#include<gtk/gtk.h>

#include "base_defs.h"
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

  float income=0.0, housing=0.0, food=0.0, transport=0.0, entertain=0.0, health=0.0, work=0.0, other=0.0;
  float total=0.0;

  printf( "  >> E %s L%4d   , pall_hdls=%p \n" , __FUNCTION__ , __LINE__, pall_hdls );

  if ( pall_hdls->vbox_chart_page != NULL ) {
    if ( pall_hdls->t_lst != NULL ) {
      float inc_per, hou_per, foo_per, tpr_per , ent_per ,hlt_per , wrk_per, otr_per;
      pokane_grp list = pall_hdls->t_lst;

      while ( list ) {
        switch ( list->category ) {
          case INCOME_ID:
            income += list->amount;
            break;
          case HOUSING_ID:
            housing += list->amount;
            break;
          case FOOD_ID:
            food += list->amount;
            break;
          case TRANSPORT_ID:
            transport += list->amount;
            break;
          case ENTERTAIN_ID:
            entertain += list->amount;
            break;
          case HEALTH_ID:
            health += list->amount;
            break;
          case WORK_ID:
            work += list->amount;
            break;
          default:
            /* other */
            other += list->amount;
            break;
        }
        total += list->amount;
        list = list->next;
      }  // while

      inc_per = income/total;
      hou_per = housing/total;
      foo_per = food/total;
      if ( foo_per < 5.0 ) {
        other += food;
        food  = 0.0;
      }
      tpr_per = transport/total;
      if ( tpr_per < 5.0 ) {
        other += transport;
        transport  = 0.0;
      }
      ent_per = entertain/total;
      if ( ent_per < 5.0 ) {
        other += entertain;
        entertain  = 0.0;
      }
      hlt_per = health/total;
      if ( hlt_per < 5.0 ) {
        other += health;
        health  = 0.0;
      }
      wrk_per = work/total;
      if ( wrk_per < 5.0 ) {
        other += work;
        work  = 0.0;
      }

      otr_per = other/total;

      printf( "  inc_per = %4.2f hou_per = %4.2f foo_per = %4.2f tpr= %4.2f ent = %4.2f hlth = %4.2f wrk = %4.2f Other = %4.2f \n" ,
					       inc_per , hou_per , foo_per, tpr_per, ent_per, hlt_per , wrk_per , otr_per );

      pie_widget_add_slice_to_pie ( (PieWidget *) pall_hdls->vbx_hdls->cp_myPie , inc_per, "#20f020", "Income");
      pie_widget_add_slice_to_pie ( (PieWidget *) pall_hdls->vbx_hdls->cp_myPie , hou_per, "#FF6484", "Housing");
      if ( foo_per > 0.0 ) 	pie_widget_add_slice_to_pie ( (PieWidget *) pall_hdls->vbx_hdls->cp_myPie , foo_per, "#FFC686", "Food");
      //pie_widget_add_slice_to_pie ( (PieWidget *) pall_hdls->vbx_hdls->cp_myPie , tpr_per, "#36A282", "Tport");
      if ( tpr_per > 0.0 ) pie_widget_add_slice_to_pie ( (PieWidget *) pall_hdls->vbx_hdls->cp_myPie , tpr_per, "#f00000", "Tport");
      if ( ent_per > 0.0 ) pie_widget_add_slice_to_pie ( (PieWidget *) pall_hdls->vbx_hdls->cp_myPie , ent_per, "#1010A0", "Entertainment");
      if ( hlt_per > 0.0 ) pie_widget_add_slice_to_pie ( (PieWidget *) pall_hdls->vbx_hdls->cp_myPie , hlt_per, "#10A0A0", "Health");
      if ( wrk_per > 0.0 ) pie_widget_add_slice_to_pie ( (PieWidget *) pall_hdls->vbx_hdls->cp_myPie , wrk_per, "#A010A0", "Work");

      pie_widget_add_slice_to_pie ( (PieWidget *) pall_hdls->vbx_hdls->cp_myPie , otr_per, "#1010A0", "Other");

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
      printf( "  >> E %s \n" , __FUNCTION__ );
      printf( "    flgs->dbg = %d \n" , pall_hdls->flg->dbg );
      printf( "    parentWin = %p \n" , pall_hdls->parentWin );
    }
  }
  else {
     printf( "  >> E %s  , pall_hdls = NULL !!! \n" , __FUNCTION__ );
  }

  if ( pall_hdls->vbox_chart_page == NULL ) {
    GtkWidget *hbox , *button;

    pall_hdls->vbox_chart_page = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);

    printf( "    vbox = %p  , %s L%4d \n" , pall_hdls->vbox_chart_page , __FILE__ , __LINE__  );

    pall_hdls->vbx_hdls->cp_myPie = pie_widget_new();

    gtk_box_pack_start( GTK_BOX( pall_hdls->vbox_chart_page ), pall_hdls->vbx_hdls->cp_myPie,  FALSE, TRUE, 0);

    hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_pack_end (GTK_BOX ( pall_hdls->vbox_chart_page ), hbox, FALSE, FALSE, 0);

    button = gtk_button_new_with_mnemonic ("_Done");
    pall_hdls->vbx_hdls->hp_chart_done_btn = button;

    g_signal_connect (button, "clicked", G_CALLBACK ( cancel_clicked ), (gpointer) pall_hdls  );

    gtk_box_pack_start (GTK_BOX (hbox), button, FALSE, FALSE, 0);

    pie_widget_set_text( (PieWidget *)pall_hdls->vbx_hdls->cp_myPie , GLG_TITLE_T , "NEW Title" );

    calc_stats( pall_hdls );

    // add ref to widget so it doesn't get destroyed on container_remove !
    g_object_ref ( pall_hdls->vbox_chart_page );

  }
 else {
    gtk_container_add (GTK_CONTAINER ( pall_hdls->parentWin ), pall_hdls->vbox_chart_page );
  }

  if ( pall_hdls->vbx_hdls->cp_myPie == NULL ) {

    pall_hdls->vbx_hdls->cp_myPie = pie_widget_new();

    gtk_box_pack_start( GTK_BOX( pall_hdls->vbox_chart_page ), pall_hdls->vbx_hdls->cp_myPie,  FALSE, TRUE, 0);
    pie_widget_set_text( (PieWidget *)pall_hdls->vbx_hdls->cp_myPie , GLG_TITLE_T , "NEW Title" );

    calc_stats( pall_hdls );
  }

  gtk_widget_show_all ( pall_hdls->vbox_chart_page );

  if ( pall_hdls->flg->dbg ) printf( "  Lv  %s pall_hdls =%p \n" , __FUNCTION__ , pall_hdls );

  return rc;
} // create_pie_chart_page

int create_pie_chart_page_rtn( phdl_grp *all_hdls ) {
  int rc = 0;
  phdl_grp pall_hdls = *all_hdls;

  printf( "  E  %s  ,  *all_hdls = %p pall_hdls =%p \n" , __FUNCTION__ , *all_hdls , pall_hdls );
  rc = create_pie_chart_page( pall_hdls );

  if ( pall_hdls != NULL ) {
    printf( "      pall_hdls->vbox_transact_page = %p \n" , pall_hdls->vbox_chart_page );
  }
  *all_hdls = pall_hdls;

  if ( pall_hdls->flg->dbg ) printf( "  Lv %s  *all_hdls = %p pall_hdls =%p \n" , __FUNCTION__ , *all_hdls , pall_hdls );
  return rc;
}

