#ifndef _TRANSACT_LIST_VIEW_H

#define _TRANSACT_LIST_VIEW_H 1

#include "base_defs.h"
#include "data_types.h"

typedef enum { POPUP_MENU_0=0 , POPUP_MENU_1, POPUP_MENU_2 , POPUP_MENU_3 , MAX_POPUP_MENU } popup_menu_id;

enum
{
  MODEL_ENTRY_NR = 0,  // INT
  MODEL_CATEGORY,      // INT   , ie money group see enum CATOG_ID
  MODEL_ENTRY_TS,      // INT
  MODEL_DATE_STR,      // FLOAT
  MODEL_AMOUNT,        // FLOAT
  MODEL_DESCRIPTION,   // STRING
  MODEL_IS_IN_DB,      // INT
  MODEL_SHARE_NUM_COLS
} ;

#define MODEL_SHARE_END_LIST -1

typedef struct
{
  GtkTreeStore       *t_act;
  GtkTreeModelSort   *sorted;
  GtkTreeModelFilter *filtered;
  float              max_xx;
} transact_lst_store , *ptransact_lst_store;

extern int add_to_trans_list_treestore ( phdl_grp pall_hdls );
extern void update_trans_list_treeview( pokane_grp head );

extern void                 create_and_fill_trans_list ( phdl_grp pall_hdls , int dbg );

extern GtkWidget          * create_trans_listview ( phdl_grp pall_hdls , int dbg );

extern void               * create_trans_list_store ( phdl_grp pall_hdls , int dbg );

#endif
