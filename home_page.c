#include <stdio.h>
#include<gtk/gtk.h>

#include "base_defs.h"

int start_dbg = 0;

int create_home_screen ( phdl_grp pall_hdls ) {

  GtkWidget *vbox, *hbox , *hbox2 , *hbox3, *hbox_spc;
  GtkWidget *label, *blnk_label;
  GtkWidget *button;

  // int rc = 0;

  // printf( "  >> E %s \n" , __FUNCTION__ );

  // if ( pall_hdls->flg->dbg ) {
  //   printf( "  >> E %s \n" , __FUNCTION__ );
  //   printf( "    flgs->dbg = %d \n" , pall_hdls->flg->dbg );
  //   printf( "    parentWin = %p \n" , pall_hdls->parentWin );
  // }
  //actions = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);

  if ( pall_hdls == NULL ) {
    printf( "  >> E %s  , all_hdls = NULL !!! \n" , __FUNCTION__ );
    return -1;
  }

  if ( start_dbg ) {
    printf( "  >> E %s \n" , __FUNCTION__ );
    printf( "    parentWin = %p \n" , pall_hdls->parentWin );
  }
  
  pall_hdls->vbox_four = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add (GTK_CONTAINER ( pall_hdls->parentWin ), pall_hdls->vbox_four);

  printf( "    vbox = %p \n" , pall_hdls->vbox_four );

  hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);

  label = gtk_label_new (" $3,261 ");

  gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, FALSE, 0);

  gtk_widget_show_all (hbox);

  gtk_container_add (GTK_CONTAINER ( pall_hdls->vbox_four ), hbox );

  hbox2 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);

  // simple button , TODO make a text_view
  button = gtk_button_new_with_label ("Show more details");
  gtk_widget_set_valign ( button, GTK_ALIGN_CENTER);
  gtk_box_pack_start (GTK_BOX ( hbox2 ), button, TRUE, FALSE, 0);

  gtk_container_add (GTK_CONTAINER ( pall_hdls->vbox_four ), hbox2 );

  hbox_spc   = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);
  blnk_label = gtk_label_new ("");
  gtk_box_pack_start (GTK_BOX (hbox_spc), blnk_label, TRUE, FALSE, 0);

  gtk_container_add (GTK_CONTAINER ( pall_hdls->vbox_four ), hbox_spc );

  hbox_spc   = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);
  blnk_label = gtk_label_new ("");
  gtk_box_pack_start (GTK_BOX (hbox_spc), blnk_label, TRUE, FALSE, 0);

  gtk_container_add (GTK_CONTAINER ( pall_hdls->vbox_four ), hbox_spc );

  hbox3 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);

  button = gtk_button_new_with_label ("Calendar");

  gtk_box_pack_start (GTK_BOX ( hbox3 ), button, TRUE, FALSE, 0);

  button = gtk_button_new_with_label ("Chart");

  gtk_box_pack_start (GTK_BOX ( hbox3 ), button, TRUE, FALSE, 0);

  button = gtk_button_new_with_label ("Calculate");

  gtk_box_pack_start (GTK_BOX ( hbox3 ), button, TRUE, FALSE, 0);

  gtk_container_add (GTK_CONTAINER ( pall_hdls->vbox_four ), hbox3 );

  gtk_widget_show_all ( pall_hdls->vbox_four );

  if ( pall_hdls->flg->dbg ) {
    printf( "  << Lv %s \n" , __FUNCTION__ );
  }

  printf( "  << Lv %s \n" , __FUNCTION__ );

  return 0;
}
