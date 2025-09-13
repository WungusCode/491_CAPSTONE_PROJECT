#include <gtk/gtk.h>
#include "setting_page.h"
#include "base_defs.h"

int setting_page_creation(void){
  int rc = 0;
  //create a new window called settings 
  gtkWidget *window;
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Settings");

  printf("Settings page created\n");

  # create the notfication box
  GtkWidget *check = gtk_check_button_new_with_label("Enable Notifications");
  gtk_container_add(GTK_CONTAINER(window), check);

  gtk_widget_show_all(window);

  printf(""<< Lv %s\n"", __function__);
  return rc;
}

 void destory_setting_page(void){
   printf("Settings page destroyed\n");
 } 
  

