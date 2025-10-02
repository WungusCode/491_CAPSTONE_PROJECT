#include <stdio.h>
#include <string.h>

#include "base_defs.h"

#include "link_list.h"
#include "my_time.h"

#include "transaction_list_page.h"

#define STEP_INC 0.2
#define MAX_PRICE_START 600

const GdkColor lgray   = RGB(231,231,231);
const GdkColor lyellow = RGB(255,255,224);
const GdkColor lpurple = RGB(203,195,227);
const GdkColor lgreen  = RGB(232,255,232);
const GdkColor lred    = RGB(255,220,220);
const GdkColor lorange = RGB(255,233,201);

typedef struct
{
  GtkTreeStore       *t_act;
  GtkTreeModelSort   *sorted;
  GtkTreeModelFilter *filtered;
  float              max_xx;
} transact_lst_store;

int g_dbg_trans_listtree    = 0;
int g_trans_listhide_no_div = 0;

transact_lst_store *g_trans_liststore      = NULL;

GtkTreeStore       *g_trans_list_treestore = NULL;
GtkWidget          *g_trans_listview       = NULL;

transact_lst_store * create_trans_list_store ( pokane_grp stk_lst , int dbg );

void set_trans_list_store( transact_lst_store *local_store ) {
  g_trans_liststore = local_store;
}

transact_lst_store  * get_trans_list_store( void ) {
  // g_print( "  %s store=%p \n" , __FUNCTION__ , g_trans_liststore );
  return g_trans_liststore;
}

void set_trans_list_treeview( GtkWidget *local_view ) {
  g_trans_listview = local_view;
  if ( g_dbg_trans_listtree ) g_print( "  %s treeview=%p \n" , __FUNCTION__ , g_trans_listview );
}

GtkWidget  * get_trans_list_treeview( void ) {
  if( g_dbg_trans_listtree ) g_print( "  %s treeview=%p \n" , __FUNCTION__ , g_trans_listview );
  return g_trans_listview;
}

void set_trans_list_treestore( GtkTreeStore *local_treestore ) {
  // g_print( "  %s treestore=%p local_treestore=%p\n" , __FUNCTION__ , g_trans_listtreestore , local_treestore);
  g_trans_list_treestore = local_treestore;
  // g_print( "  %s NOW treestore=%p \n" , __FUNCTION__ , g_trans_listtreestore );
}

GtkTreeStore  * get_trans_list_treestore( void ) {
  // g_print( "  %s treestore=%p \n" , __FUNCTION__ , g_trans_listtreestore );
  return g_trans_list_treestore;
}

// render funcs --

void paint_background_trans_list ( GtkCellRenderer   *renderer,
                    GtkTreeModel      *model,
                    GtkTreeIter       *iter,
                    int               u_dat) {

  int stk_mth=0;

  gtk_tree_model_get(model, iter, MODEL_CATEGORY , &stk_mth, -1);
  /* set 'cell-background' property of the cell renderer */
  switch( stk_mth ) {
    // This could be replaced with if else, but case allows future flexibility
    case 1:
      g_object_set(renderer, "cell-background-gdk", &lgreen, "cell-background-set", TRUE, NULL);
      break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
      g_object_set(renderer, "cell-background-gdk", &lorange, "cell-background-set", TRUE, NULL);
      break;
    default:
      g_object_set(renderer,
            "cell-background", "white",
            "cell-background-set", FALSE,  NULL);
      break;
  }

  g_object_set(renderer, "height", 20, NULL);

} // paint_background

int gen_render_needs_coding_warn = 0;

int g_TODO_Warning = 0;

void gen_renderer_func_trans_list ( GtkTreeViewColumn *col,
                    GtkCellRenderer   *renderer,
                    GtkTreeModel      *model,
                    GtkTreeIter       *iter,
                    gpointer           user_data) {

  int u_dat = (intptr_t)user_data;

  if ( u_dat == MODEL_CATEGORY ) {
    if ( !gen_render_needs_coding_warn ) printf( "  %s  THIS NEEDS  coding %s !! , %s L%4d \n" , COLOR_YELLOW , COLOR_RESET , __func__, __LINE__  );
    gen_render_needs_coding_warn++;
  }

  if ( u_dat == MODEL_ENTRY_TS ) {
    if ( !g_TODO_Warning ) printf( "    %sTODO: %sconvert uint32_ts to date time string ! , %s L%d \n" , COLOR_YELLOW, COLOR_RESET , __func__ , __LINE__ );
   g_TODO_Warning = 1;
  }
  if ( u_dat == MODEL_AMOUNT ) {
    float amount;
    gchar  buf[64];

    gtk_tree_model_get(model, iter, MODEL_AMOUNT , &amount, -1);

    g_snprintf(buf, sizeof(buf), "%7.2f", amount );

    g_object_set(renderer, "foreground-set", FALSE, NULL); /* print this normal */

    g_object_set(renderer, "text", buf, NULL);

  }
  paint_background_trans_list ( renderer, model, iter, u_dat );

} // gen_renderer_func_trans_list


gboolean update_parent_only_trans_list (GtkTreeModel *model,
                GtkTreePath  *path,
                GtkTreeIter  *iter,
                gpointer      user_data)
{
  //int  dbg = get_debug();
  int  dbg = 1;
//dbg=1;
  if ( dbg ) {
    printf( "\n  E %s L%d , model=%p iter=%p \n" , __FUNCTION__, __LINE__ , model, iter );
  }

  printf( "\n\n    %s L%4d   , update this !! \n\n" , __FUNCTION__ , __LINE__ ); 

  if ( dbg ) printf( "  Lv %s L%d \n\n" , __FUNCTION__, __LINE__  );

  return TRUE; // only do first one , shld be parent !
}

gboolean update_prices_foreach_trans_list (GtkTreeModel *model,
                GtkTreePath  *path,
                GtkTreeIter  *iter,
                gpointer      user_data)
{
  int       entry_nr, category, is_in_dB;
  uint32_t  entry_ts;
  float     amount;
  gchar    *description;
  gchar    *tree_path_str;
  char     date_str[31];

  int   ii, chgd = 0;
  int 	dbg = 1;
  pokane_grp tmp = NULL;
  pokane_grp head = ( gpointer) user_data;

  transact_lst_store  *store = NULL;

  printf( "   >> E %s  TODO - add code to it ! L%4d   store=%p \n" , __func__ , __LINE__ , store );

  printf( "    head-> entry_nr=%4d category=%2d entry_ts=%08x amount=%6.2f descrip=%s in_dB=%d \n" , head->entry_nr, head->category
                                                                                           , head->entry_ts, head->amount
                                                                                           , head->description , head->in_dB );
  tree_path_str = gtk_tree_path_to_string(path);
  printf( "      tree_path=%s \n" , tree_path_str );
  store         = get_trans_list_store();
//dbg=1;

  if ( dbg ) {
    g_print( "  E update_prices_foreach  "  );
  }

  gtk_tree_model_get (model, iter, MODEL_ENTRY_NR            , &entry_nr    , -1 );
  gtk_tree_model_get (model, iter, MODEL_CATEGORY            , &category    , -1 );
  gtk_tree_model_get (model, iter, MODEL_ENTRY_TS            , &entry_ts    , -1 );
  gtk_tree_model_get (model, iter, MODEL_DATE_STR            , &date_str    , -1 );
  gtk_tree_model_get (model, iter, MODEL_AMOUNT              , &amount      , -1 );
  gtk_tree_model_get (model, iter, MODEL_DESCRIPTION         , &description , -1 );
  gtk_tree_model_get (model, iter, MODEL_IS_IN_DB            , &is_in_dB    , -1 );

  printf( "  MODEL_ENTRY_NR = %4d , find in okane list !! \n" , entry_nr );
  tmp = head;
  for ( ii=0; ii < entry_nr && head != NULL ; ii++ ) {
    tmp = tmp->next;
  }
  printf( "  FOUND: \n" );
  printf( "    tmp-> entry_nr=%4d category=%2d entry_ts=%08x date_str=%9s amount=%6.2f descrip=%s in_db=%d \n" , tmp->entry_nr, tmp->category
                                                                                           , tmp->entry_ts, "--/--/--" , tmp->amount
                                                                                           , tmp->description , tmp->in_dB );


  if ( tmp->category != category ) { category  = tmp->category;  chgd=1; }
  if ( tmp->amount    != amount  ) { amount    = tmp->amount;     chgd=1; }

  if ( chgd == 1 ) {
    gtk_tree_store_set ( GTK_TREE_STORE (model), iter,
                         MODEL_CATEGORY , category,
                         MODEL_AMOUNT    , amount,
                        -1);

  }
  getchar();

  printf( "  THIS NEEDS CODING !!! %s L%4d   store=%p \n" , __func__ , __LINE__ , store );
  g_free( tree_path_str);
  g_free( description );

  printf( "   << Lv %s  L%4d   \n" , __func__ , __LINE__ );
  return FALSE;
} // update_trans_listprices_foreach

// end render funcs

// callbacks

void update_trans_list_treeview( pokane_grp head ) {

  GtkWidget           *view;
  int dbg = 1 /* get_debug() */ ;
  if ( dbg ) printf( "\n  %s file %s L%d , print updated list :\n" , __FUNCTION__ , __FILE__ , __LINE__ );

  gtk_tree_model_foreach( GTK_TREE_MODEL( get_trans_list_treestore() ), update_prices_foreach_trans_list , (gpointer)head );

  view = get_trans_list_treeview();
  gtk_widget_queue_draw( GTK_WIDGET( view ) ); // refresh treeview

  if ( dbg ) printf( "  Lv %s file %s , L%d \n" , __FUNCTION__ , __FILE__, __LINE__ );
}

static void done_clicked ( GtkButton *button,  gpointer   user_data) {
  // ALWAYS called from home_page, so go back to it !
  phdl_grp all_hdls = (phdl_grp)user_data;

  gtk_container_remove ( GTK_CONTAINER ( all_hdls->parentWin ) , all_hdls->vbox_t_history_page );

  // Go back to home page screen
  gtk_container_add ( GTK_CONTAINER ( all_hdls->parentWin ) , all_hdls->vbox_home_page );
  gtk_widget_show_all ( all_hdls->parentWin );
}

int add_to_trans_list_treestore ( pokane_grp head ) {

  GtkTreeIter iter;
  pokane_grp tmp = NULL;

  GtkTreeStore *treestore=NULL;
  char time_str[31];

  treestore = get_trans_list_treestore( );

  tmp = head;
  // goto end of list, only works for add one at a time ! TODO, allow multiple
  while( tmp->next != NULL ) {
    tmp = tmp->next;
  }

  printf( "  FOUND: \n" );
  printf( "    tmp-> entry_nr=%4d category=%2d entry_ts=%08x amount=%6.2f descrip=%s \n" , tmp->entry_nr, tmp->category
                                                                                           , tmp->entry_ts, tmp->amount
                                                                                           , tmp->description );
  gtk_tree_store_append( treestore, &iter, NULL);

  convert_to_date_string( tmp->entry_ts , time_str );
  gtk_tree_store_set ( treestore, &iter,
                      MODEL_ENTRY_NR    , tmp->entry_nr ,
                      MODEL_CATEGORY    , tmp->category ,
                      MODEL_ENTRY_TS    , tmp->entry_ts ,
                      MODEL_DATE_STR    , time_str,
                      MODEL_AMOUNT      , tmp->amount,
                      MODEL_DESCRIPTION , tmp->description,
                      MODEL_IS_IN_DB    , tmp->in_dB,
                      MODEL_SHARE_END_LIST);
  printf( "   %s  , L%4d    added new entry ! \n" , __func__, __LINE__ );
  //getchar();

  return 0;
}  // add_to_trans_list_treestore

// end callbacks

int row_visible_warn = 0;

static gboolean transact_list_row_visible (GtkTreeModel *model, GtkTreeIter *iter, transact_lst_store *store) {

  int rc  = 1;
  int dbg = 1 /* get_debug() */;

  if ( dbg && ( row_visible_warn < 2 ) ) g_print( "    >> E %s L%d iter=%p store->articles (treestore) =%p \n" , __FUNCTION__ , __LINE__ , iter , store->t_act);

  if ( row_visible_warn < 2 ) printf( "        %s - code needs implementing ! , L%4d \n" , __FUNCTION__ , __LINE__ );
  row_visible_warn++;

  if ( dbg && ( row_visible_warn < 2 ) ) g_print( "    << Lv %s L%d \n" , __func__ , __LINE__ );
  return rc;
}  // transact_list_row_visible

void create_and_fill_trans_list ( pokane_grp stk_lst ) {

  GtkTreeIter    toplevel;
#ifdef COPIED_FROM_HEADER_TODO_REMOVE
  GtkTreeIter     child;
#endif
  GtkTreeStore *treestore=NULL;
  char time_str[31];

  pokane_grp list = stk_lst;

  //int dbg = get_debug();
  int dbg = 1;
//dbg=1;
  if ( dbg ) printf( "    >> E  %s L%d \n" , __FUNCTION__ , __LINE__ );

  treestore = get_trans_list_treestore( );

  if ( dbg) {
    if (  list != NULL ) {
      printf( "        In %s lnk lst entry_nr=%d amount=%f descrip=%s \n" , __FUNCTION__ , list->entry_nr, list->amount, list->description );
    }
    else {
      printf("    stk_lst = %p \n", stk_lst);
    }
  }

  if ( dbg ) {
    linked_list_print_okane_grp ( stk_lst );
  }

  /* Append a top level row and leave it empty */
  gtk_tree_store_append(treestore, &toplevel, NULL);

  while ( list != NULL ) {
    convert_to_date_string( list->entry_ts , time_str );

		gtk_tree_store_set ( treestore, &toplevel,
                      MODEL_ENTRY_NR    , list->entry_nr ,
                      MODEL_CATEGORY   , list->category ,
                      MODEL_ENTRY_TS    , list->entry_ts ,
                      MODEL_DATE_STR    , time_str ,
                      MODEL_AMOUNT      , list->amount,
                      MODEL_DESCRIPTION , list->description,
                      MODEL_IS_IN_DB    , list->in_dB,
                      MODEL_SHARE_END_LIST);

    if ( dbg ) {
      printf( "    Nr=%3d category=%d entry_ts=%8x date=%s amnt=%6.2f desc=%s in_dB=%d \n" , list->entry_nr, list->category, list->entry_ts
                                                                                    , time_str , list->amount  , list->description, list->in_dB );
    }
    list = list->next;
    if ( list ) {
      gtk_tree_store_append ( treestore, &toplevel, NULL );
    }

  }  // while list != NULL

  if ( dbg ) printf( "    << Lv %s L%d \n" , __FUNCTION__ , __LINE__ );

  return;
} // create_and_fill_trans_list

void toggle_row(GtkCellRendererToggle *cell_renderer, gchar *path,  GtkTreeStore *model )
{

  GtkTreeIter        iter;
  GValue             toggle_state = G_VALUE_INIT;
  transact_lst_store  *store = NULL;

  GtkTreePath *orig_path = gtk_tree_path_new_from_string( path );

  GtkTreePath *filtered_path;
  GtkTreePath *child_path;

  //int  dbg = get_debug();
  int  dbg = 1;

  store = get_trans_list_store();

  if ( dbg ) printf( " E  %s path=%s orig_path=%p \n" , __FUNCTION__ , path , orig_path );

  g_value_init(&toggle_state, G_TYPE_INT);

  // get correct iter !
  filtered_path = gtk_tree_model_sort_convert_path_to_child_path (GTK_TREE_MODEL_SORT (store->sorted),  orig_path);
  child_path    = gtk_tree_model_filter_convert_path_to_child_path (GTK_TREE_MODEL_FILTER (store->filtered), filtered_path);

  if ( dbg ) printf( "  %s filtered_path=%s child_path=%s \n" , __FUNCTION__ , gtk_tree_path_to_string ( filtered_path ) , gtk_tree_path_to_string ( child_path ) );

  if ( gtk_tree_model_get_iter (GTK_TREE_MODEL ( model ), &iter, child_path ) ) {

    if ( store->max_xx == MAX_PRICE_START ) {
      float cur_price=0;

      store->max_xx = cur_price;
    }

  }  // if get iter of child_path

  if ( dbg ) printf( "\n  %s file %s L%d , toggle_row , 1st call to update_prices_foreach_trans_list \n" , __FUNCTION__ , __FILE__ , __LINE__ );

  gtk_tree_model_foreach( GTK_TREE_MODEL( get_trans_list_treestore() ) , update_prices_foreach_trans_list , NULL /* (gpointer)one_stk */ );

  // Need to do twice to update the average
  if ( dbg ) printf( "\n  %s file %s L%d , toggle_row , 2nd call to update_parent_only_trans_list \n" , __FUNCTION__ , __FILE__ , __LINE__ );
  gtk_tree_model_foreach( GTK_TREE_MODEL( get_trans_list_treestore() ) , update_parent_only_trans_list , NULL );

  if ( dbg ) printf( "\n  %s file %s L%d , DONE gtk_tree_model_foreach() \n" , __FUNCTION__ , __FILE__ , __LINE__ );

  if ( dbg )   printf( "  Lv %s L%d \n" , __FUNCTION__ , __LINE__ );
}  // toggle_row


void check_box (GtkCellRendererToggle * cell, gchar * path, GtkTreeStore * model) {
    GtkTreeIter iter;
    gboolean active;

    active = gtk_cell_renderer_toggle_get_active (cell);

    gtk_tree_model_get_iter_from_string (GTK_TREE_MODEL (model), &iter, path);

    if (active) {
        gtk_cell_renderer_set_alignment(GTK_CELL_RENDERER(cell), 0, 0);
        gtk_tree_store_set (GTK_TREE_STORE (model), &iter, 0, FALSE, -1);
    }
    else {
        gtk_cell_renderer_set_alignment(GTK_CELL_RENDERER(cell), 0.5, 0.5);
        gtk_tree_store_set (GTK_TREE_STORE (model), &iter, 0, TRUE, -1);
    }
} // check_box

static GtkWidget * create_trans_listview ( transact_lst_store *store , int dbg ) {
  GtkWidget           *view;
  GtkCellRenderer     *renderer;
  GtkTreeViewColumn   *col;

  int                 user_dat=0;

#ifdef HAVE_RENDER_DEFINED
  GtkTreeSelection    *selection;
#endif
  view = get_trans_list_treeview( );

  if ( dbg ) printf( "  >> E %s L%d \n" , __FUNCTION__ , __LINE__ );

  g_object_unref ( store->sorted ); /* destroy model automatically with view */

#ifdef HAVE_RENDER_DEFINED
  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));

  /* set up a slect . un-selected action ---> see view_selected_func */
  gtk_tree_selection_set_select_function(selection, view_selection_func, NULL, NULL);
#endif

#ifdef USE_ROW_TOOLTIP
  // set up floating tooltips - this applies to ROWS !
  g_object_set ( view, "has-tooltip", TRUE, NULL);
#ifdef CONNECT_SIGNALS
  g_signal_connect ( view, "query-tooltip", G_CALLBACK (query_tooltip_tree_view_cb), NULL);
#endif

#endif

  // Don't need as, automatically set when 'sortable' set
  //gtk_tree_view_set_headers_clickable( GTK_TREE_VIEW( view ) , TRUE);

#ifdef CONNECT_SIGNALS
  /* Connect signals */
  g_signal_connect(view, "button-press-event", G_CALLBACK(view_onButtonPressed), NULL);
  g_signal_connect(view, "popup-menu", G_CALLBACK(view_onPopupMenu), NULL);
#endif

  /* --- Column #1 --- */
  col = gtk_tree_view_column_new();

  gtk_tree_view_column_set_title(col, "Entry Nr");

  /* make tock_name a sortable column ! */
  gtk_tree_view_column_set_sort_column_id (col, MODEL_ENTRY_NR );

  gtk_tree_view_column_set_max_width ( col, 70);

  /* pack tree view column into tree view */
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
  // floating tooltip for column header only
  g_object_set ( gtk_tree_view_column_get_button (col), "tooltip-text", "transaction number", NULL);

  renderer = gtk_cell_renderer_text_new();

  /* pack cell renderer into tree view column */
  gtk_tree_view_column_pack_start(col, renderer, TRUE);

  /* connect 'text' property of the cell renderer to
   *  model column that contains the first name */
  gtk_tree_view_column_add_attribute(col, renderer, "text", MODEL_ENTRY_NR );

  user_dat = MODEL_ENTRY_NR;
  gtk_tree_view_column_set_cell_data_func(col, renderer, gen_renderer_func_trans_list,  (void *)(intptr_t)user_dat , NULL /* GtkDestroyNotify destroy */ );

  /* --- Column #2 --- */

  col = gtk_tree_view_column_new();
  if ( col == NULL ) printf( "Oh my !\n" );
  gtk_tree_view_column_set_max_width ( col, 50);

  gtk_tree_view_column_set_title(col, "Cat");

  g_object_set ( gtk_tree_view_column_get_button (col), "tooltip-text", "category of expense, 1 = money received, >=2 different types of expense, eg food housing etc", NULL);

  /* pack tree view column into tree view */
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);

  renderer = gtk_cell_renderer_text_new();

  /* pack cell renderer into tree view column */
  gtk_tree_view_column_pack_start(col, renderer, TRUE);

  /* connect 'text' property of the cell renderer to
   *  model column that contains the first name */
  gtk_tree_view_column_add_attribute(col, renderer, "text", MODEL_CATEGORY );
  gtk_tree_view_column_set_sort_column_id (col, MODEL_CATEGORY );

  user_dat = MODEL_CATEGORY;
  gtk_tree_view_column_set_cell_data_func(col, renderer, gen_renderer_func_trans_list,  (void *)(intptr_t)user_dat , NULL /* GtkDestroyNotify destroy */ );

  /* --- Column #3 --- */
  col = gtk_tree_view_column_new();
  if ( col == NULL ) printf( "Oh my !\n" );

  gtk_tree_view_column_set_title(col, "Date");
  g_object_set ( gtk_tree_view_column_get_button (col), "tooltip-text", "Date of transaction", NULL);

  /* pack tree view column into tree view */
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);

  renderer = gtk_cell_renderer_text_new();

  /* pack cell renderer into tree view column */
  gtk_tree_view_column_pack_start(col, renderer, TRUE);

  /* connect 'text' property of the cell renderer to
   *  model column that contains the first name */
  gtk_tree_view_column_add_attribute(col, renderer, "text", MODEL_ENTRY_TS );
  user_dat = MODEL_ENTRY_TS;
  gtk_tree_view_column_set_cell_data_func(col, renderer, gen_renderer_func_trans_list,  (void *)(intptr_t)user_dat , NULL /* GtkDestroyNotify destroy */ );

  gtk_tree_view_column_set_sort_column_id (col, MODEL_ENTRY_TS );

  /* --- Column #4 --- */
  col = gtk_tree_view_column_new();
  if ( col == NULL ) printf( "Oh my !\n" );

  gtk_tree_view_column_set_title(col, "Date");
  g_object_set ( gtk_tree_view_column_get_button (col), "tooltip-text", "Date String", NULL);

  /* pack tree view column into tree view */
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);

  renderer = gtk_cell_renderer_text_new();

  /* pack cell renderer into tree view column */
  gtk_tree_view_column_pack_start(col, renderer, TRUE);

  /* connect 'text' property of the cell renderer to
   *  model column that contains the first name */
  gtk_tree_view_column_add_attribute(col, renderer, "text", MODEL_DATE_STR );
  user_dat = MODEL_ENTRY_TS;
  gtk_tree_view_column_set_cell_data_func(col, renderer, gen_renderer_func_trans_list,  (void *)(intptr_t)user_dat , NULL /* GtkDestroyNotify destroy */ );

  gtk_tree_view_column_set_sort_column_id (col, MODEL_DATE_STR );

  /* --- Column #5 --- */
  col = gtk_tree_view_column_new();
  if ( col == NULL ) printf( "Oh my !\n" );
  gtk_tree_view_column_set_max_width ( col, 90);

  gtk_tree_view_column_set_title(col, "Amount");
  g_object_set ( gtk_tree_view_column_get_button (col), "tooltip-text", "Amount : how much was received or paid", NULL);

  /* make AMOUNT a sortable column ! */
  gtk_tree_view_column_set_sort_column_id (col, MODEL_AMOUNT);

  /* pack tree view column into tree view */
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);

  renderer = gtk_cell_renderer_text_new();
  g_object_set (renderer, "xalign", 0.5, NULL);

  /* pack cell renderer into tree view column */
  gtk_tree_view_column_pack_start(col, renderer, TRUE);

  /* connect 'text' property of the cell renderer to
   *  model column that contains the first name */
  gtk_tree_view_column_add_attribute(col, renderer, "text", MODEL_AMOUNT );
  user_dat = MODEL_AMOUNT;

  gtk_tree_view_column_set_cell_data_func(col, renderer, gen_renderer_func_trans_list,  (void *)(intptr_t)user_dat , NULL /* GtkDestroyNotify destroy */ );

  /* --- Column #6 --- */
  col = gtk_tree_view_column_new();
  if ( col == NULL ) printf( "Oh my !\n" );
  gtk_tree_view_column_set_max_width ( col, 350);

  gtk_tree_view_column_set_title(col, "Description");
  g_object_set ( gtk_tree_view_column_get_button (col), "tooltip-text", "Description, eg wages if a credit, gasoline if a debit", NULL);

  /* make DESCRIPTION a sortable column ! */
  gtk_tree_view_column_set_sort_column_id (col, MODEL_DESCRIPTION );

  /* pack tree view column into tree view */
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
  renderer = gtk_cell_renderer_text_new();

  /* pack cell renderer into tree view column */
  gtk_tree_view_column_pack_start(col, renderer, TRUE);

  gtk_tree_view_column_add_attribute(col, renderer, "text", MODEL_DESCRIPTION );
  user_dat = MODEL_DESCRIPTION;
  gtk_tree_view_column_set_cell_data_func(col, renderer, gen_renderer_func_trans_list,  (void *)(intptr_t)user_dat , NULL /* GtkDestroyNotify destroy */ );

  /* --- Column #7 --- */

  // TODO : make this a hidden column
  col = gtk_tree_view_column_new();
  if ( col == NULL ) printf( "Oh my !\n" );
  gtk_tree_view_column_set_max_width ( col, 50);

  gtk_tree_view_column_set_title(col, "in dB");
  g_object_set ( gtk_tree_view_column_get_button (col), "tooltip-text", "Does record exist in database ? ( has data been saved )", NULL);

  /* pack tree view column into tree view */
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);

  renderer = gtk_cell_renderer_text_new();

  /* pack cell renderer into tree view column */
  gtk_tree_view_column_pack_start(col, renderer, TRUE);

  /* connect 'text' property of the cell renderer to
   *  model column that contains the first name */
  gtk_tree_view_column_add_attribute(col, renderer, "text", MODEL_IS_IN_DB );
  user_dat = MODEL_IS_IN_DB;
  gtk_tree_view_column_set_cell_data_func(col, renderer, gen_renderer_func_trans_list,  (void *)(intptr_t)user_dat , NULL /* GtkDestroyNotify destroy */ );

  return view;
} // create_trans_listview

transact_lst_store * create_trans_list_store ( pokane_grp stk_lst , int dbg ) {

  transact_lst_store *store = NULL;
  GtkWidget        *view;

  if ( dbg ) g_print( "  >> E %s L%d  \n" , __FUNCTION__ , __LINE__ );

  store = g_new0 ( transact_lst_store, 1);
  store->max_xx = MAX_PRICE_START;

  store->t_act = gtk_tree_store_new ( MODEL_SHARE_NUM_COLS ,
       G_TYPE_INT,        // MODEL_ENTRY_NR
       G_TYPE_INT,        // MODEL_CATEGORY
       G_TYPE_INT,        // MODEL_ENTRY_TS     // TODO make hidden
       G_TYPE_STRING,     // MODEL_DATE_STR
       G_TYPE_FLOAT,      // MODEL_AMOUNT
       G_TYPE_STRING,     // MODEL_DESCRIPTION
       G_TYPE_INT         // IS_IN_DB
  );

  if ( dbg ) g_print( "  NR_COLS=%d done gtk_tree_store_new \n" , MODEL_SHARE_NUM_COLS );

  set_trans_list_store( store );

  g_print( "  NR_COLS=%d done gtk_tree_store_new \n" , MODEL_SHARE_NUM_COLS );

  if (  store->t_act == NULL ) {
    g_print( "  NR_COLS=%d stg wrong with treestore \n" , MODEL_SHARE_NUM_COLS );
    getchar();
  }
  set_trans_list_treestore( store->t_act );
  if ( dbg ) {
    linked_list_print_okane_grp ( stk_lst );
  }

  // model is actually GTK_TREE_MODEL(treestore)
  create_and_fill_trans_list ( stk_lst );

  // store->t_act is the base model, order is :
  //  t_act
  //    filtered
  //      sorted  <-- view is derived from this
  store->filtered = GTK_TREE_MODEL_FILTER (gtk_tree_model_filter_new (GTK_TREE_MODEL (store->t_act), NULL));
  store->sorted   = GTK_TREE_MODEL_SORT (gtk_tree_model_sort_new_with_model (GTK_TREE_MODEL (store->filtered))); // store->sorted is the model being used with view !

  gtk_tree_model_filter_set_visible_func (store->filtered, (GtkTreeModelFilterVisibleFunc) transact_list_row_visible, store, NULL);

  view = gtk_tree_view_new_with_model ( GTK_TREE_MODEL ( store->sorted ));  // view is of sorted model, not the original 't_act' model

  // ALSO GTK_TREE_VIEW_GRID_LINES_NONE ,GTK_TREE_VIEW_GRID_LINES_HORIZONTAL, GTK_TREE_VIEW_GRID_LINES_VERTICAL
  gtk_tree_view_set_grid_lines( GTK_TREE_VIEW ( view ), GTK_TREE_VIEW_GRID_LINES_BOTH );

  set_trans_list_treeview( view );

  if ( dbg ) g_print( "  << Lv %s L%d  \n" , __FUNCTION__ , __LINE__ );

  return store;
} // create_trans_list_store

int create_transaction_history_page( phdl_grp pall_hdls ) {

  int rc = 0;

  if ( pall_hdls != NULL ) {
    if ( pall_hdls->flg->dbg ) {
      printf( "  >> E %s \n" , __FUNCTION__ );
      printf( "    flgs->dbg     = %d \n" , pall_hdls->flg->dbg );
      printf( "    parentWin     = %p \n" , pall_hdls->parentWin );
      printf( "    vbox_transact = %p \n" , pall_hdls->vbox_t_history_page );
    }
  }
  else {
      printf( "  >> E %s pall_hdls = NULL  \n" , __FUNCTION__ );
  }

  if ( pall_hdls->vbox_t_history_page == NULL ) {
    GtkWidget *frame, *vbox;
    GtkWidget  *view;
    GtkWidget  *hbox, *button;
    GtkWidget  *scrolledwindow, *table;

    // tree stuff
    transact_lst_store         *store;

    pokane_grp    head        = pall_hdls->t_lst;
    pokane_grp    one_tkr_lst = head;  // this correct ??

    pall_hdls->vbox_t_history_page = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    printf( "    vbox_t_history_page = %p   , %s L%4d \n" , pall_hdls->vbox_t_history_page , __FILE__ , __LINE__ );

/* LAYOUT
   |-------------------------------------------|
   | vbox                                      |
   |   hbox                                    |
   |     frame                                 |
   |       table                               |
   |         scrolledwindow                    |
   |           view                            |
   |             sorted list                   |
   |   hbox                                    |
   |     button                                |
   |-------------------------------------------|
*/
		if ( pall_hdls->vbox_active != NULL ) {
      // only attach, if no vbox active !
      gtk_container_add (GTK_CONTAINER ( pall_hdls->parentWin ), pall_hdls->vbox_t_history_page );
    }

    frame = gtk_frame_new ("Trans List");
    gtk_widget_set_size_request( frame, 400,300 );
    gtk_container_set_border_width (GTK_CONTAINER ( frame ), 1);

    gtk_container_add (GTK_CONTAINER ( pall_hdls->vbox_t_history_page ), frame);

    vbox   = gtk_box_new ( GTK_ORIENTATION_VERTICAL, 12);
    gtk_container_add (GTK_CONTAINER (frame) , vbox);

    table = gtk_table_new( 20,1, FALSE /* TRUE */);        // 6x6 table to be put in frame
    gtk_widget_set_size_request( table, 400,250 );

    gtk_box_pack_start (GTK_BOX ( vbox ), table, FALSE, FALSE, 0);

    scrolledwindow = gtk_scrolled_window_new(NULL, NULL );
    gtk_widget_show( scrolledwindow );
    gtk_table_attach(GTK_TABLE(table), scrolledwindow, 0,2,0,12, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL ,0,0 );

    // make the tree list
    store = create_trans_list_store ( one_tkr_lst , pall_hdls->flg->dbg );

    view = GTK_WIDGET ( create_trans_listview ( store , pall_hdls->flg->dbg ) );
    set_trans_list_treeview( view );
    gtk_container_add(GTK_CONTAINER(scrolledwindow), view );      // add view to scrolled window

		hbox   = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);
    button = gtk_button_new_with_label ("DONE");
    g_signal_connect (button, "clicked", G_CALLBACK ( done_clicked ), (gpointer) pall_hdls  );
    gtk_box_pack_start (GTK_BOX ( hbox ), button, FALSE, FALSE, 0);

    // for auto test
    pall_hdls->vbx_hdls->tlp_list_trans_done_btn = button;
    gtk_box_pack_end (GTK_BOX ( vbox ), hbox, FALSE, FALSE, 0);

    // add ref to widget so it doesn't get destroyed on container_remove !
    g_object_ref ( pall_hdls->vbox_t_history_page );
  } // !if ( pall_hdls->vbox_t_history_page == NULL
  else {
    gtk_container_add (GTK_CONTAINER ( pall_hdls->parentWin ), pall_hdls->vbox_t_history_page );
  }
  gtk_widget_show_all ( pall_hdls->vbox_t_history_page );

  if ( pall_hdls->flg->dbg ) {
    printf( "  << Lv %s \n" , __FUNCTION__ );
  }
  return rc;
} // create_transaction_history_page

int create_transaction_history_page_rtn( phdl_grp *all_hdls ) {
  int rc = 0;
  phdl_grp pall_hdls = *all_hdls;

  printf( "  >> E  %s  *all_hdls = %p pall_hdls =%p \n" , __FUNCTION__ , *all_hdls , pall_hdls );
  rc = create_transaction_history_page( pall_hdls );

  if ( pall_hdls != NULL ) {
    printf( "      pall_hdls->vbox_transact_page = %p \n" , pall_hdls->vbox_t_history_page );
  }
  *all_hdls = pall_hdls;

  printf( "  << Lv %s  *all_hdls = %p pall_hdls =%p \n" , __FUNCTION__ , *all_hdls , pall_hdls );
  return rc;
}  // create_transaction_history_page_rtn

