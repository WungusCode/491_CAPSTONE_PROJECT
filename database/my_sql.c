
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
int sql_log_dbg    = 1;

void set_log_sql_access ( int logit ) {
  log_sql_access = logit;
}

int get_log_sql_access ( void ) {
  return log_sql_access;
}

sqlite3 *dbInit(int create_db, char *dbName ) {
  sqlite3 *db;
  int rt = sqlite3_open( dbName, &db);

  printf ( "  E %s \n" , __FUNCTION__ );
  if ( log_sql_access ) printf( "     SQL_LOG: %s L%d called sqlite3_open rc=%d \n" , __FUNCTION__ , __LINE__ , rt );

  if (rt != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return NULL;
  }

  if (create_db) {
    int rc = 0;
    char *errmsg;

    char *sql_1 = "CREATE TABLE IF NOT EXISTS okane_record ( transaction_Id INTEGER PRIMARY KEY AUTOINCREMENT,is_income INTEGER NOT NULL,transact_ts INT,amount REAL, description VARCHAR(255) NOT NULL )";

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
  printf( "  Lv %s , db=%p \n" , __FUNCTION__ , db );
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

  if ( dbg ) printf( "  << E %s \n" , __FUNCTION__ );

  if ( db ) sqlite3_close( db );
  sqlite3_shutdown( ); // needed ??

  if ( dbg ) printf( "  << Lv %s \n" , __FUNCTION__ );
  return 0;
}

sqlite3 *db_open( const char * dbName , const char * from_where, int db_nr , int dbg ) {

  sqlite3 *db;
  int      rt;
  int pause = 0;

  dbg |= sql_log_dbg;

  if ( dbg ) printf( "      E %s called dbName=%s from %s db_nr=%2d \n" , __FUNCTION__ , dbName , from_where , db_nr );

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

  if ( dbg ) printf( "      %s called from %s , db now OPEN !\n", __FUNCTION__  , from_where );

  return db;
}

int db_add_entry( sqlite3 *db_hdl , pokane_grp data_lst , int dbg ) {

  int ii, rc=0;
  int insert_errors=0;

  const char *sql_end  = "INSERT OR IGNORE INTO okane_record (  `transaction_id`, `is_income`, `transact_ts`, `amount`, `description`) VALUES ( ";

  char t_id[11];
  char is_income[11];
  char entry_ts[11];
  char amount[11];
  char descrip[255];

	char error_str[255];
  char db_string[512];

  char *errmsg = error_str;

  strcpy( db_string , sql_end );

  ii  = 0;
  while( data_lst ) {
	  strcpy( db_string , sql_end );

    sprintf( t_id , "%d, "         , data_lst->entry_nr    );
    strcat( db_string , t_id );

    sprintf( is_income , "%d, "    , data_lst->is_income   );
    strcat( db_string , is_income );

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
    data_lst = data_lst->next;
    ii++;
    memset ( db_string, 0 , sizeof( db_string ) );
  } // while
  if ( 1 ) printf( "  ii=%4d data_lst=%p \n" , ii, data_lst );

  return rc;
}
