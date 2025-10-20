#ifndef _BUDGET_PAGE_H

#define _BUDGET_PAGE_H 1

#include <gtk/gtk.h>
#include "base_defs.h"
#include "data_types.h"

GtkWidget* create_budget_page(void);

extern long g_budget_value;
extern char *g_budget_string;
#endif