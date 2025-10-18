#include <stdio.h>
#include <stdlib.h>
#include "indent_print.h"

#include "link_list.h"

int ll_dbg         = 1;

#if 0
int g_dbg_all_hdls = 1;

hdl_grp g_all_hdls;

void set_all_hdls ( phdl_grp all_hdls ) {

  g_all_hdls.parentWin          = all_hdls->parentWin;
  g_all_hdls.vbox_home_page     = all_hdls->vbox_home_page;
  g_all_hdls.vbox_transact_page = all_hdls->vbox_transact_page;

  //TODO figure out how to do flags bit
//  g_all_hdls.flg->dbg            = all_hdls->flg->dbg;
}

void set_all_hdls_from ( phdl_grp all_hdls , const char * called_from , int lineNr ) {

  if ( g_dbg_all_hdls ) printf( "  E %s , called from %s , L%3d \n" , __FUNCTION__ , called_from , lineNr );
  set_all_hdls ( all_hdls );
  if ( g_dbg_all_hdls ) printf( "  Lv %s , called from %s  \n" , __FUNCTION__ , called_from  );

}

phdl_grp get_all_hdls ( void ) {
  if ( g_dbg_all_hdls ) printf( "  %s &g_all_hdls=%p \n" , __FUNCTION__ , (void *)&g_all_hdls );
  return &g_all_hdls;
}
#endif

int get_okane_grp_list_len( pokane_grp head ) {
   int length = 0;
   pokane_grp current;

   if ( head != NULL ) {
     for(current = head; current != NULL; current = current->next) {
       length++;
     }
   }
   return length;
}

pokane_grp linked_list_add_okane_grp( pokane_grp head, pokane_grp newP) {

  if ( ll_dbg ) printf( "  >> E %s \n" , __FUNCTION__ );

  if (head == NULL) {
    head = newP;
    if ( ll_dbg ) printf( "    set head = newP = %p \n" , newP );
  } else {
    pokane_grp tmp = head;
    while (tmp->next != NULL) {
      tmp = tmp->next;
    }
    tmp->next = newP;
    if ( ll_dbg ) printf( "    head = %p newP = %p \n" , head , newP );
  }

  if ( ll_dbg ) printf( "  << Lv %s \n" , __FUNCTION__ );

  return head;
}

pokane_grp linked_list_empty_okane_grp( pokane_grp head) {
  pokane_grp tmp = head;
  pokane_grp tmp2 = head->next;
  while (tmp2 != NULL) {
    free(tmp);
    tmp = tmp2;
    tmp2 = tmp2->next;
  }
  head = NULL;
  return head;
}

/**Print the contents of the list */
void linked_list_print_okane_grp( pokane_grp head) {
  pokane_grp tmp = head;

  if ( ll_dbg ) LOG_BLOCK_START ( "  >> E %s \n" , __FUNCTION__ );

  LOG_INDENTED ("    printing list...\n");
  LOG_INDENTED ("      entry_ts \t   entry_nr is_inc \tamount  \tdescription         \t in_dB \n");
  while (tmp != NULL) {
    LOG_INDENTED("      %u\t    %d \t%d \t%7.2f   \t%-20s \t %d \n", tmp->entry_ts, tmp->entry_nr, tmp->category, tmp->amount, tmp->description , tmp->in_dB );
    tmp = tmp->next;
  }

  if ( ll_dbg ) LOG_BLOCK_END ( "  << Lv %s \n" , __FUNCTION__ );
}

