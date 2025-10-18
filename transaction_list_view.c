#include <stdio.h>
#include <string.h>

#include "base_defs.h"
#include "indent_print.h"

#include "link_list.h"
#include "my_time.h"

#include "transaction_list_view.h"

#define STEP_INC 0.2
#define MAX_PRICE_START 600

const GdkColor lgray   = RGB(231,231,231);
const GdkColor lyellow = RGB(255,255,224);
const GdkColor lpurple = RGB(203,195,227);
const GdkColor lgreen  = RGB(232,255,232);
const GdkColor lred    = RGB(255,220,220);
const GdkColor lorange = RGB(255,233,201);

// New colours
const GdkColor sageGreen = RGB( 0xc2 ,0xd6,0xc2);
const GdkColor lightLavender = RGB( 0xe6 , 0xe6 , 0xff );
const GdkColor paleYellow    = RGB( 0xff , 0xff , 0xe6 );
const GdkColor paleAqua      = RGB( 0xd6 , 0xf5 , 0xf5);
const GdkColor palePeach     = RGB( 0xff , 0xe6 , 0xcc );

const GdkColor pastelPink    = RGB( 0xff , 0xe0 , 0xe6 );

int g_dbg_trans_listtree    = 0;
int g_trans_listhide_no_div = 0;

void * create_trans_list_store ( phdl_grp pall_hdls , int dbg );

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
    case INCOME_ID :
      //g_object_set(renderer, "cell-background-gdk", &lgreen, "cell-background-set", TRUE, NULL);
      g_object_set(renderer, "cell-background-gdk", &lgreen, "cell-background-set", TRUE, NULL);
      break;
    case HOUSING_ID :
      g_object_set(renderer, "cell-background-gdk", &lightLavender, "cell-background-set", TRUE, NULL);
      break;
		case UTIL_ID :
      g_object_set(renderer, "cell-background-gdk", &paleYellow, "cell-background-set", TRUE, NULL);
      break;
		case FOOD_ID :
      g_object_set(renderer, "cell-background-gdk", &paleAqua, "cell-background-set", TRUE, NULL);
      break;
    case TRANSPORT_ID :
      g_object_set(renderer, "cell-background-gdk", &palePeach, "cell-background-set", TRUE, NULL);
      break;
    case ENTERTAIN_ID :
    case HEALTH_ID :
    case WORK_ID   :
      g_object_set(renderer, "cell-background-gdk", &lorange, "cell-background-set", TRUE, NULL);
      break;
    case OTHER_ID :
      g_object_set(renderer, "cell-background-gdk", &pastelPink, "cell-background-set", TRUE, NULL);
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

// end render funcs

// callbacks

int add_to_trans_list_treestore ( phdl_grp pall_hdls ) {

  GtkTreeIter iter;
  pokane_grp tmp = NULL;

  GtkTreeStore *treestore=NULL;

	char time_str[31];

  LOG_BLOCK_START ( "  >> E  %s  L%4d  pall_hdls=%p \n" , __FUNCTION__ , __LINE__ , pall_hdls );

	treestore = GTK_TREE_STORE ( pall_hdls->vbx_hdls->tlp_treeStore );
  tmp = pall_hdls->t_lst;

  // goto end of list, only works for add one at a time ! TODO, allow multiple
  while( tmp->next != NULL ) {
    tmp = tmp->next;
  }
  convert_to_date_string( tmp->entry_ts , time_str );

  LOG_INDENTED ( "  FOUND: \n" );
  LOG_INDENTED ( "    tmp-> entry_nr=%4d category=%2d entry_ts=%08x Date=%s amount=%6.2f descrip=%s \n" , tmp->entry_nr, tmp->category
                                                                                           , tmp->entry_ts, time_str, tmp->amount
                                                                                           , tmp->description );
  gtk_tree_store_append( treestore, &iter, NULL);

  gtk_tree_store_set ( treestore, &iter,
                      MODEL_ENTRY_NR    , tmp->entry_nr ,
                      MODEL_CATEGORY    , tmp->category ,
                      MODEL_ENTRY_TS    , tmp->entry_ts ,
                      MODEL_DATE_STR    , time_str,
                      MODEL_AMOUNT      , tmp->amount,
                      MODEL_DESCRIPTION , tmp->description,
                      MODEL_IS_IN_DB    , tmp->in_dB,
                      MODEL_SHARE_END_LIST);
  LOG_INDENTED ( "   %s  , L%4d    added new entry ! \n" , __func__, __LINE__ );

	LOG_BLOCK_END ( "  << Lv  %s  L%4d , pall_hdls=%p  \n" , __FUNCTION__ , __LINE__ , pall_hdls );
  //getchar();

  return 0;
}  // add_to_trans_list_treestore

// end callbacks

int row_visible_warn = 0;

static gboolean tlv_row_visible (GtkTreeModel *model, GtkTreeIter *iter, transact_lst_store *store) {

  int rc  = 1;
  int dbg = 1 /* get_debug() */;

  if ( dbg && ( row_visible_warn < 2 ) ) LOG_BLOCK_START( "    >> E %s L%d iter=%p store->articles (treestore) =%p \n" , __FUNCTION__ , __LINE__ , iter , store->t_act);

  if ( row_visible_warn < 2 ) LOG_INDENTED ( "        %s - code needs implementing ! , L%4d \n" , __FUNCTION__ , __LINE__ );
  row_visible_warn++;

  if ( dbg && ( row_visible_warn < 2 ) ) LOG_BLOCK_END ( "    << Lv %s L%d \n" , __func__ , __LINE__ );
  return rc;
}  // tlv_row_visible

void create_and_fill_trans_list ( phdl_grp pall_hdls , int dbg ) {

  GtkTreeIter    toplevel;
#ifdef COPIED_FROM_HEADER_TODO_REMOVE
  GtkTreeIter     child;
#endif
  GtkTreeStore *treestore=NULL;
  char time_str[31];

  pokane_grp list = pall_hdls->t_lst;

  dbg = 0;

  if ( dbg ) LOG_BLOCK_START ( ">> E  %s L%d \n" , __FUNCTION__ , __LINE__ );

  treestore = pall_hdls->vbx_hdls->tlp_treeStore;
  if ( dbg ) LOG_INDENTED ( "  In %s treestore=%p \n" , __FUNCTION__ , treestore );
  //get_trans_list_treestore( );

  if ( dbg ) LOG_INDENTED ( "  In %s lnk lst entry_nr=%d amount=%f descrip=%s \n" , __FUNCTION__ , list->entry_nr, list->amount, list->description );

  if ( dbg ) {
    linked_list_print_okane_grp ( pall_hdls->t_lst );
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
      LOG_INDENTED ( "    Nr=%3d category=%d entry_ts=%8x date=%s amnt=%6.2f desc=%25s in_dB=%d \n" , list->entry_nr, list->category, list->entry_ts
                                                                                    , time_str , list->amount  , list->description, list->in_dB );
    }
    list = list->next;
    if ( list ) {
      gtk_tree_store_append ( treestore, &toplevel, NULL );
    }

  }  // while list != NULL

  if ( dbg ) LOG_BLOCK_END ( "    << Lv %s L%d \n" , __FUNCTION__ , __LINE__ );

  return;
} // create_and_fill_trans_list

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

GtkWidget * create_trans_listview ( phdl_grp pall_hdls , int dbg ) {
  GtkWidget           *view;
  GtkCellRenderer     *renderer;
  GtkTreeViewColumn   *col;

  int                 user_dat=0;

#ifdef HAVE_RENDER_DEFINED
  GtkTreeSelection    *selection;
#endif
  transact_lst_store *store = pall_hdls->vbx_hdls->tlp_t_lst_store;

  //view = get_trans_list_treeview( );
  //view = pall_hdls->vbx_hdls->tlp_treeView;
  view = pall_hdls->vbx_hdls->tlp_treeView;

  if ( dbg ) LOG_BLOCK_START ( "  >> E %s L%d \n" , __FUNCTION__ , __LINE__ );

  g_object_unref ( store->sorted ); /* destroy model automatically with view */
  LOG_INDENTED ( "  view = %p pall_hdls->vbx_hdls->tlp_treeView = %p \n" , view , pall_hdls->vbx_hdls->tlp_treeView );

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

  if ( dbg ) LOG_BLOCK_END ( "  << Lv view=%p %s L%d  \n" , view , __FUNCTION__ , __LINE__ );

	return view;
} // create_trans_listview

void * create_trans_list_store ( phdl_grp pall_hdls , int dbg ) {

  transact_lst_store *store = NULL;
  GtkWidget        *view;

  if ( dbg ) LOG_BLOCK_START ( "  >> E %s L%d  \n" , __FUNCTION__ , __LINE__ );

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

  if ( dbg ) LOG_INDENTED ( "  NR_COLS=%d done gtk_tree_store_new \n" , MODEL_SHARE_NUM_COLS );

  pall_hdls->vbx_hdls->tlp_t_lst_store = (void *)store;

	LOG_INDENTED ( "  store = transact_lst_store = %p \n" , store  );

  LOG_INDENTED ( "  NR_COLS=%d done gtk_tree_store_new \n" , MODEL_SHARE_NUM_COLS );

  if (  store->t_act == NULL ) {
    LOG_INDENTED ( "  NR_COLS=%d stg wrong with treestore \n" , MODEL_SHARE_NUM_COLS );
    getchar();
  }
  pall_hdls->vbx_hdls->tlp_treeStore = store->t_act;
  //LOG_INDENTED ( "%s  dbg=%d list_treestore=%p , tlp_treeStore=%p %s \n" , COLOR_YELLOW , dbg, store->t_act  , pall_hdls->vbx_hdls->tlp_treeStore , COLOR_RESET );
  //getchar();

	if ( 0 /* dbg */ ) {
    //linked_list_print_okane_grp ( stk_lst );
    linked_list_print_okane_grp ( pall_hdls->t_lst );
  }

  // model is actually GTK_TREE_MODEL(treestore)
  create_and_fill_trans_list ( pall_hdls , dbg );

  // store->t_act is the base model, order is :
  //  t_act
  //    filtered
  //      sorted  <-- view is derived from this
  store->filtered = GTK_TREE_MODEL_FILTER (gtk_tree_model_filter_new (GTK_TREE_MODEL (store->t_act), NULL));
  store->sorted   = GTK_TREE_MODEL_SORT   (gtk_tree_model_sort_new_with_model (GTK_TREE_MODEL (store->filtered))); // store->sorted is the model being used with view !

  gtk_tree_model_filter_set_visible_func (store->filtered, (GtkTreeModelFilterVisibleFunc) tlv_row_visible, store, NULL);

  view = gtk_tree_view_new_with_model ( GTK_TREE_MODEL ( store->sorted ));  // view is of sorted model, not the original 't_act' model

  // ALSO GTK_TREE_VIEW_GRID_LINES_NONE ,GTK_TREE_VIEW_GRID_LINES_HORIZONTAL, GTK_TREE_VIEW_GRID_LINES_VERTICAL
  gtk_tree_view_set_grid_lines( GTK_TREE_VIEW ( view ), GTK_TREE_VIEW_GRID_LINES_BOTH );

  // set_trans_list_treeview( view );
  pall_hdls->vbx_hdls->tlp_treeView = view;
  LOG_INDENTED ( "  view = %p pall_hdls->vbx_hdls->tlp_treeView =%p \n" , view, pall_hdls->vbx_hdls->tlp_treeView  );

  if ( dbg ) LOG_BLOCK_END ( "  << Lv %s L%d  \n" , __FUNCTION__ , __LINE__ );

  return ( void * )store;
} // create_trans_list_store

