
#include <fcntl.h>            /* Definition of AT_* constants */
#include <unistd.h>

#include "../base_defs.h"
#include "../link_list.h"
#include "../my_time.h"

#include "my_sql.h"


/* Create the SQLite tables if needed (if createdb is true), and return
 * the SQLite database handle. Return NULL on error.
*/

int log_sql_access = 0;
int sql_log_dbg    = 0;

void set_log_sql_access ( int logit ) {
  log_sql_access = logit;
}

int get_log_sql_access ( void ) {
  return log_sql_access;
}

sqlite3 *dbInit(int create_db, char *dbName ) {
  sqlite3 *db;
  int rt = sqlite3_open( dbName, &db);

  printf ( "    >> E %s \n" , __FUNCTION__ );
  if ( log_sql_access ) printf( "     SQL_LOG: %s L%d called sqlite3_open rc=%d \n" , __FUNCTION__ , __LINE__ , rt );

  if (rt != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return NULL;
  }

  if (create_db) {
    int rc = 0;
    char *errmsg;

    char *sql_1 = "CREATE TABLE IF NOT EXISTS okane_record ( transaction_Id INTEGER PRIMARY KEY AUTOINCREMENT,category INTEGER NOT NULL,transact_ts INT,amount REAL, description VARCHAR(255) NOT NULL )";

    printf( "  sql is ->%s<- \n , %s L%d " , sql_1 , __FILE__ , __LINE__ );
    rc = sqlite3_exec(db, sql_1, 0, 0, &errmsg);
    if ( log_sql_access ) printf( "     SQL_LOG: %s L%d called sqlite3_exec rc=%d \n" , __FUNCTION__ , __LINE__ , rc );

    if (rc != SQLITE_OK) {
      fprintf(stderr, "SQL error [%d]: %s\n", rc, errmsg);
      sqlite3_free(errmsg);
      sqlite3_close(db);
      return NULL;
    }

  }
  printf( "    << Lv %s , db=%p \n" , __FUNCTION__ , db );
  return db;
}

int create_new_db( char *dbName ) {

  sqlite3* db;

  int dbg = 1;


  db = dbInit( 1 /* int createdb */, dbName );
  if ( db == NULL ) {
    printf( "    Something WRONG ! db=%p \n" , db );
  }
  else {
    db_close( db , __FUNCTION__ , dbg );
  }
  return 0;
}

int db_close( sqlite3 *db , const char * from_where , int dbg ) {

  dbg |= sql_log_dbg;

  if ( dbg ) printf( "    >> E %s , called from %s \n" , __FUNCTION__ , from_where );

  if ( db ) sqlite3_close( db );
  sqlite3_shutdown( ); // needed ??

  if ( dbg ) printf( "    << Lv %s \n" , __FUNCTION__ );
  return 0;
}

sqlite3 *db_open( const char * dbName , const char * from_where, int db_nr , int dbg ) {

  sqlite3 *db;
  int      rt;
  int pause = 0;

  dbg |= sql_log_dbg;

  if ( dbg ) printf( "   >> E %s called dbName=%s from %s db_nr=%2d \n" , __FUNCTION__ , dbName , from_where , db_nr );

  if (access( dbName, F_OK) != 0) {
    printf( "      db file %s DOESN'T exist ! ent to cont ( %s L%d ) :" , dbName , __FILE__ , __LINE__ );
    if ( pause ) getchar();
    return NULL;
  }
  rt = sqlite3_open( dbName, &db);
  if (rt != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    printf( "      %s file=%s called from %s , db open FAILED !\n", __FUNCTION__  , dbName , from_where );
    return NULL;
  }

  if ( dbg ) printf( "   << Lv %s called from %s , db now OPEN !\n", __FUNCTION__  , from_where );

  return db;
}

int db_add_entry( sqlite3 *db_hdl , pokane_grp data_lst , int dbg ) {

  int ii, rc=0;
  int insert_errors=0;

  const char *sql_end  = "INSERT OR IGNORE INTO okane_record (  `transaction_id`, `category`, `transact_ts`, `amount`, `description`) VALUES ( ";

  char t_id[11];
  char category[11];
  char entry_ts[21];
  char amount[11];
  char descrip[ DB_DESCRIP_LEN + 5 ];

  char error_str[255];
  char db_string[512];

  char *errmsg = error_str;

  strcpy( db_string , sql_end );

  if ( dbg ) printf( "   >> E %s dbHdl=%p dbStr=%s  \n" , __func__ , db_hdl , db_string );

  ii  = 0;
  while( data_lst ) {
    if ( dbg ) printf( "      entry_nr = %3d , amnt=%6.2f desc=%20s indB=%d \n" , data_lst->entry_nr, data_lst->amount, data_lst->description, data_lst->in_dB );

    if ( data_lst->in_dB == 0 ) {
      // for now DON'T try to re-add entries already known to be in the database
      strcpy( db_string , sql_end );

      sprintf( t_id , "%d, "         , data_lst->entry_nr    );
      strcat( db_string , t_id );

      sprintf( category , "%d, "    , data_lst->category   );
      strcat( db_string , category );

      sprintf( entry_ts  , "%u, "    , data_lst->entry_ts    );
      strcat( db_string , entry_ts );

      sprintf( amount    , "%7.2f, " , data_lst->amount      );
      strcat( db_string , amount );

      sprintf( descrip   , "'%s' );"   , data_lst->description );
      strcat( db_string , descrip );

      if ( dbg ) {
        printf( " db_str=%3u , ->%s<- \n" , (unsigned)strlen( db_string ), db_string );
      }
      rc = sqlite3_exec( db_hdl, db_string , NULL /* sql_callback */ , 0, &errmsg);

      if ( log_sql_access ) {
        printf( "          query = %s \n" , db_string );
        printf( "     SQL_LOG: %s L%d called sqlite3_exec rc=%d \n" , __FUNCTION__ , __LINE__ , rc );
      }

      if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error [%d]: %s\n", rc, errmsg);
        insert_errors++;
        if ( insert_errors > 10 && !(insert_errors % 50) ) {
          printf( "  insert errors %d idx=%d ! %s %s %d \n", insert_errors, ii, __FUNCTION__ , __FILE__ , __LINE__  );
          getchar();
        }
      }
      data_lst->in_dB = 1;
    }  // if NOT in_db

    data_lst = data_lst->next;
    ii++;
    memset ( db_string, 0 , sizeof( db_string ) );
  } // while
  if ( 1 ) printf( "  ii=%4d data_lst=%p \n" , ii, data_lst );

  return rc;
}

int db_read_in_all_transactions( sqlite3 *db_hdl , pokane_grp *data_lst, int dbg ) {
  int rc = 0;

  int nrTkr=0;  // rename to nrRecords

  int idx;
  sqlite3_stmt *stmt;
  int row_limit = 1;
  int val_i     = 0;
  double val_d  = 0;

  char sql_query[128] = "";
  char sql_base[] = "SELECT * FROM okane_record;" ;  // get all data

  char *sql_str;

  pokane_grp head = NULL;

  dbg |= sql_log_dbg;

  if ( dbg ) printf( "  E %s L%d file %s , *data_lst=%p head=%p dbg=%u  \n" , __FUNCTION__ , __LINE__ , __FILE__ , *data_lst , head , dbg );

  strcpy( sql_query, sql_base );
  sql_str = sql_query;

  if ( dbg ) {
    printf( "  sql_str = ->%s<- \n" , sql_str );
    // getchar();
  }

  if ( row_limit == 0 ) row_limit = 10000;

  rc = sqlite3_prepare_v2( db_hdl , sql_str , -1, &stmt, NULL);
  if ( log_sql_access ) printf( "     SQL_LOG: %s L%d called sqlite3_prepare_v2 rc=%d\n" , __FUNCTION__ , __LINE__  , rc );

  if ( dbg ) printf("  %s L%d Got results:\n" , __FUNCTION__ , __LINE__ );
  idx = 0;

  while (sqlite3_step(stmt) != SQLITE_DONE) {
    int      ii;
    int      no_data     = 0;
    int      category    = 0;
    uint32_t transact_ts = 0;
    float    amount      = 0.0;

    int num_cols = sqlite3_column_count(stmt);

    char descrip [ DB_DESCRIP_LEN + 1 ];

    val_d =0; val_i=0;

    if ( dbg ) printf( "  idx=%d " , idx );

    for ( ii = 0; ii < num_cols; ii++)  {

      switch (sqlite3_column_type(stmt, ii ))
      {
      case (SQLITE3_TEXT):
        strcpy( descrip , (const char *)sqlite3_column_text(stmt, ii) );
        if ( dbg ) printf("%s, ", descrip) ;
        break;
      case (SQLITE_INTEGER):
        val_i = sqlite3_column_int(stmt, ii);
        if ( dbg ) printf("%d, ", val_i );

        if ( ii == 1 ) category = val_i;
        else if ( ii == 2 ) transact_ts = (uint32_t)val_i;

        break;
      case (SQLITE_FLOAT):
        val_d = sqlite3_column_double(stmt, ii);
        if ( dbg ) printf("%g, ", val_d );
        amount = val_d;

        break;
      default:
        printf( " i=%d SQLITE_UNKNOWN = %d \n" , ii, (int)sqlite3_column_type(stmt, ii) );
        break;
      }
      if ( dbg ) {
        printf("\n");
        // getchar();
      }

    } // for

    pokane_grp newP = malloc( sizeof( okane_grp ) );
    memset( newP, 0 , sizeof( okane_grp) );
    newP->entry_nr    = nrTkr;
    newP->category    = category;
    newP->entry_ts    = transact_ts;
    newP->amount      = amount;
    newP->in_dB       = 1;
    strcpy( newP->description, descrip );

    if ( no_data ) printf( "\n" );

    if ( dbg ) printf( "  nr=%3d descrip=%s amount=%5.2f  ,head=%p \n" , newP->entry_nr , newP->description, newP->amount , head );

    head = linked_list_add_okane_grp( head, newP );
    nrTkr++;

    if ( no_data == 0 )   idx++;

    if ( idx && !(idx%10) ) {
#if 0
      if ( getchar_stop ) {
        printf( " idx = %d \n" , idx );
//        getchar();
      }
#endif
    }

  } // while
  if ( dbg ) printf( "  val_i=%d val_d=%g  ( %s L%d ) \n" , val_i, val_d , __FILE__ , __LINE__ );

  rc = sqlite3_finalize( stmt );
  if ( log_sql_access || rc ) printf( "     SQL_LOG: %s L%d called sqlite3_finalize rc=%d\n" , __FUNCTION__ , __LINE__  , rc );

  *data_lst = head;

  return nrTkr;
}  // db_read_in_all_transactions
