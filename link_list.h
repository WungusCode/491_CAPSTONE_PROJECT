#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H 1

#include <stdint.h>

#include "base_defs.h"

#if 0
extern void     set_all_hdls_from ( phdl_grp all_hdls , const char * called_from , int lineNr );
extern phdl_grp get_all_hdls ( void );
#endif

extern int        get_okane_grp_list_len        ( pokane_grp head );
extern pokane_grp linked_list_add_okane_grp     ( pokane_grp , pokane_grp newP );
extern pokane_grp linked_list_empty_okane_grp   ( pokane_grp head );
extern void       linked_list_print_okane_grp   ( pokane_grp head );

#endif

