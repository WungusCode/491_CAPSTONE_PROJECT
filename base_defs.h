#ifndef _BASE_DEFS_H

#define _BASE_DEFS_H 1

#include <gtk/gtk.h>
#include <glib.h>
#include <gmodule.h>

#include "data_types.h"

#define WIN_W 770
#define WIN_H 480

#define DB_DESCRIP_LEN 255
#define NR_BUDGET_CHARS 7

#define  RGB(R,G,B) {(R<<16)|(G<<8)|B,(R*65535)/255,(G*65535)/255,(B*65535)/255}

#define COLOR_BOLD   "\033[1;37m"
#define COLOR_BLACK  "\033[0;30m"
#define COLOR_RED    "\033[0;31m"
#define COLOR_GREEN  "\033[0;32m"
#define COLOR_ORANGE "\033[0;33m"
#define COLOR_YELLOW "\033[1;33m"
#define COLOR_BLUE   "\033[0;34m"
#define COLOR_PURPLE "\033[0;35m"
#define COLOR_CYAN   "\033[0;36m"
#define COLOR_WHITE  "\033[0;37m"
#define COLOR_RESET  "\033[0m"

typedef enum { NO_SELECTION_ID=0 , INCOME_ID, HOUSING_ID, UTIL_ID ,FOOD_ID, TRANSPORT_ID, ENTERTAIN_ID, HEALTH_ID, WORK_ID, OTHER_ID , MAX_ID=-1 } CATOG_ID;

typedef struct _okane_grp {
  struct _okane_grp  *next;                         // next entry in single linked list
  int                 entry_nr;                     // transaction_id in DB
  int                 category;                     // one of enum xxx
  uint32_t            entry_ts;                     // unix timestamp of when the transaction occured
  float               amount;                       // how much money was received ( income ) or spent ( debit )
  char                description[DB_DESCRIP_LEN];  // text to describe transaction, eg 'wages' etc if income, 'gas', 'food' etc if debit
                                                    // fixed alloc for now, use malloc later ??
  int                 in_dB;                        // flag to indicate this entry already exists in database
} okane_grp, * pokane_grp;

typedef struct _uiHdl {
  // home page widgets
  GtkWidget *hp_budget_btn;
 
  GtkWidget *hp_budget_label;
 
  GtkWidget *hp_plus_trans_btn;

  GtkWidget *hp_chart_btn;

  GtkWidget *hp_list_trans_btn;

  GtkWidget    *hp_treeView;
  GtkTreeStore *hp_treeStore;    // void so can pass without headers
  void         *hp_t_lst_store;  // void so can pass without headers, is transact_lst_store defined in transaction_list_view.h

  // transaction_page widgets
  GtkWidget *tp_w_is_income;
  GtkWidget *tp_w_amount;
  GtkWidget *tp_w_date;
  GtkWidget *tp_w_description;
  GtkWidget *tp_w_category;
  GtkWidget *tp_cancel_btn;       // cancel or go back button
  GtkWidget *tp_add_record_btn;   // Add record to list
  GtkWidget *tp_add_dB_btn;       // save to dB button

  // transaction list page widgets
  // GtkWidget *thp_xx;
  GtkWidget    *tlp_list_trans_done_btn;

  GtkWidget    *tlp_treeView;
  GtkTreeStore *tlp_treeStore;    // void so can pass without headers
  void         *tlp_t_lst_store;  // void so can pass without headers, is transact_lst_store defined in transaction_list_view.h

  // chart_page widgets
  GtkWidget *cp_myPie;
  GtkWidget *cp_pie_done_btn;

  // start screen_page widgets
  GtkWidget *sp_login_btn;   // login  button , move to next screen
  GtkWidget *sp_create_btn;  // create button , move to 'create account' screen
  GtkWidget *sp_exit_btn;    // exit   button , move to 'submit' screen

  // login screen_page widgets
  GtkWidget *lp_create_btn;  // create button , move to 'create account' screen
  GtkWidget *lp_submit_btn;  // subit  button , move to 'submit' screen
  GtkWidget *lp_back_btn;    // back   button  , leave app

  GtkWidget    *tp_recent_treeView;
  GtkTreeStore *tp_recent_treeStore;
  void         *tp_recent_t_lst_store;

  GtkWidget *tp_date_today_btn;  // "Today" helper button

} uiHdl, *puiHdl;

// keep at bottom
typedef struct _hdl_grp {
  GtkWidget *parentWin;

  // Keep vbox handles here for easy reference
  GtkWidget *vbox_active;         // current vbox attached to parentWin, can only be ONE at any time !
  GtkWidget *vbox_start_page;     // vbox container for 'start_page'
  GtkWidget *vbox_create_page;    // vbox container for 'create_page'
  GtkWidget *vbox_login_page;     // vbox container for 'login_page'
  GtkWidget *vbox_home_page;      // vbox container for 'home_page'
  GtkWidget *vbox_transact_page;  // vbox container for 'transact_page'
  GtkWidget *vbox_chart_page;     // vbox container for 'pie_page'
  GtkWidget *vbox_t_history_page; // vbox container for 'transaction_history_page'
  GtkWidget *vbox_budget_page;

  // collect hdls to widgets of vboxes here
  puiHdl vbx_hdls;

  papp_flags flg;

  pokane_grp t_lst;               // linked list of transactions

  int budget_val;

  int budget_val_tmp;

  char budget_str[NR_BUDGET_CHARS];

} hdl_grp, *phdl_grp;

#endif
