#include <stdio.h>
#include<gtk/gtk.h>

#include "base_defs.h"

void end_test_sequence( void ) {
  gtk_main_quit();
}

gboolean do_test_sequence_3( gpointer data ) {
  phdl_grp pall_hdls = (phdl_grp)data;

  printf( "  >> E %s , L%4d \n" , __func__, __LINE__ );

  // select 'Done' to go back to main screen
  g_print("    Injecting 'clicked' signal to go back to home page...\n");
  g_signal_emit_by_name( pall_hdls->vbx_hdls->hp_chart_done_btn , "clicked", NULL);

  g_print("    Injecting 'clicked' signal to select pie chart...\n");
  g_signal_emit_by_name( pall_hdls->vbx_hdls->hp_plus_trans_btn , "clicked", NULL);

  g_timeout_add ( 2000, (GSourceFunc)end_test_sequence, NULL );

  printf( "  << Lv %s , L%4d \n" , __func__, __LINE__ );
  return FALSE;
}

gboolean do_test_sequence_2( gpointer data ) {
  phdl_grp pall_hdls = (phdl_grp)data;

  printf( "  >> E %s , L%4d \n" , __func__, __LINE__ );

  // select 'Done' to go back to main screen
  g_print("    Injecting 'clicked' signal to go back to home page...\n");
  g_signal_emit_by_name( pall_hdls->vbx_hdls->hp_list_trans_done_btn , "clicked", NULL);

  g_print("    Injecting 'clicked' signal to select pie chart...\n");
  g_signal_emit_by_name( pall_hdls->vbx_hdls->hp_chart_btn , "clicked", NULL);

  g_timeout_add ( 1000, (GSourceFunc) do_test_sequence_3, data );

  printf( "  << Lv %s , L%4d \n" , __func__, __LINE__ );
  return FALSE;
}


gboolean do_test_sequence( gpointer data ) {
  phdl_grp pall_hdls = (phdl_grp)data;

  printf( "  >> E %s , L%4d \n" , __func__, __LINE__ );

  g_print("    Injecting 'clicked' signal into the button...\n");
  g_signal_emit_by_name( pall_hdls->vbx_hdls->hp_list_trans_btn , "clicked", NULL);

  g_timeout_add ( 1000, (GSourceFunc) do_test_sequence_2, data );

  printf( "  << Lv %s , L%4d \n" , __func__, __LINE__ );

  return FALSE;
}

