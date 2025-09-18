#ifndef _SETTING_PAGE_H_
#define _SETTING_PAGE_H_

#include <gtk/gtk.h>
#include "base_defs.h"
#include "data_types.h"

GtkWidget* create_setting_page(void);

void destroy_setting_page(GtkWidget *window);

#endif // _SETTING_PAGE_H_