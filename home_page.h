#ifndef _HOME_PAGE_H

#define _HOME_PAGE_H 1

#include "base_defs.h"
#include "data_types.h"

// int create_home_screen ( phdl_grp *all_hdls );

int create_home_screen ( phdl_grp all_hdls );
int create_home_screen_rtn( phdl_grp *pall_hdls );      // no extern duplicates 
#endif