#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H 1

#include <stdint.h>

typedef struct _okane_grp {
  struct _okane_grp  *next;             // next entry in single linked list
  int                 entry_nr;         // transaction_id in DB
  int                 is_income;        // if 0, it's a debit ( ie spending ), if 1, it's income ( wages, lottery etc )
  uint32_t            entry_ts;         // unix timestamp of when the transaction occured
  float               amount;           // how much money was received ( income ) or spent ( debit ) 
  char                description[31];  // text to describe transaction, eg 'wages' etc if income, 'gas', 'food' etc if debit
                                        // fixed alloc for now, use malloc later ??
} okane_grp, * pokane_grp;


extern int        get_okane_grp_list_len        ( pokane_grp head );
extern pokane_grp linked_list_add_okane_grp     ( pokane_grp , pokane_grp newP );
extern pokane_grp linked_list_empty_okane_grp   ( pokane_grp head );
extern void       linked_list_print_okane_grp   ( pokane_grp head );

#endif

