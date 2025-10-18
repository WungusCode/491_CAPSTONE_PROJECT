#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>

#include <time.h>  // for time() localtime()

int g_t_dbg = 0;

void print_tm_struct ( struct tm *now ) {
  printf( "  tm:\t" );
  printf( "  yr=%d mon=%d day=%d hr=%d min=%d sec=%d \n" , now->tm_year, now->tm_mon, now->tm_mday, now->tm_hour , now->tm_min , now->tm_sec );
}

int get_month_now( void ) {

  int rc,dbg=0;
  time_t tim=time(NULL);
  struct tm *now=localtime(&tim);

  dbg |= g_t_dbg;

  rc = now->tm_mon;
  rc++;
  if ( dbg ) printf( "  Mth now = %d \n" , rc );
  return rc;
}

uint32_t get_month_start_ts( void ) {

  int dbg =0;
  time_t mth_start;

  time_t tim=time(NULL);
  struct tm *now=localtime(&tim);

  dbg |= g_t_dbg;

  if ( dbg ) {
    printf( "      Raw t " );
    print_tm_struct ( now );
  }
  now->tm_mday = 1;
  now->tm_sec  = 0;
  now->tm_min  = 0;
  now->tm_hour = 0;

  if ( dbg ) {
    printf( "      Mth Start: " );
    print_tm_struct ( now );
  }

  mth_start = (time_t)mktime( now );
  if ( mth_start == -1 ) printf( "  Failed to obtain start of month timestamp ! \n" );

  return mth_start ;
}

uint32_t get_month_end_ts( void ) {

  int dbg=0;
  time_t mth_end;

  time_t tim=time(NULL);
  struct tm *now=localtime(&tim);

  dbg |= g_t_dbg;
  // set to beginning of next month
  now->tm_mday = 1;
  now->tm_sec  = 0;
  now->tm_min  = 0;
  now->tm_hour = 0;
  now->tm_mon  += 1;

  mth_end = (time_t)mktime( now );
  if ( mth_end == -1 ) printf( "  Failed to obtain start of month timestamp ! \n" );

  if ( dbg ) {
    printf( "      Mth End " );
    print_tm_struct ( now );
  }

  return mth_end ;
}

uint32_t get_unix_time_now( void ) {

  struct timeval tv;
  gettimeofday(&tv, NULL);
  // printf("  Seconds since Jan. 1, 1970: %ld\n", tv.tv_sec);

  return tv.tv_sec;
}

uint32_t get_n_month_start_ts( int Mth ) {

  int dbg=0;
  time_t mth_start;

  time_t tim=time(NULL);
//  struct tm *now=localtime(&tim);
  struct tm *now=gmtime(&tim);

  dbg |= g_t_dbg;

  Mth -= 1;

  if ( dbg ) print_tm_struct ( now );

  // set to beginning of next month
  now->tm_mday =  1;
  now->tm_sec  =  1;
  now->tm_min  = 20;
  now->tm_hour =  0;
  if ( Mth >= 0 )  now->tm_mon  = Mth;

  if ( dbg ) printf( "  file=%s L%d Mth = %d now->tm_mon  = %d \n" , __FILE__ , __LINE__ , Mth , now->tm_mon );

  //mth_start = (time_t)timegm( now );
#ifdef _WIN32
    mth_start = (time_t)_mkgmtime(now);
#else
    mth_start = (time_t)timegm(now);
#endif

  if ( mth_start == -1 ) printf( "  Failed to obtain start of month timestamp ! \n" );

  if ( dbg ) {
    printf( "      Mth Start " );
    print_tm_struct ( now );
  }

  return mth_start ;
}

uint32_t get_n_month_end_ts( int Mth ) {

  int dbg=0;
  time_t mth_end;

  time_t tim=time(NULL);
//  struct tm *now=localtime(&tim);
  struct tm *now=gmtime(&tim);

  dbg |= g_t_dbg;

  if ( dbg ) print_tm_struct ( now );

  // set to beginning of next month
  now->tm_mday = 1;
  now->tm_sec  = 0;
  now->tm_min  = 0;
  now->tm_hour = 0;
  if ( Mth >= 0 )  now->tm_mon  = Mth;
  else now->tm_mon  += 1;

  if ( dbg ) printf( "  file=%s L%d Mth = %d now->tm_mon  = %d \n" , __FILE__ , __LINE__ , Mth , now->tm_mon );

  mth_end = (time_t)mktime( now );
  if ( mth_end == -1 ) printf( "  Failed to obtain start of month timestamp ! \n" );
  mth_end -= 60;

  if ( dbg ) {
    printf( "      Mth End " );
    print_tm_struct ( now );
  }

  return mth_end ;
}

void convert_to_date_string( float unix_ts , char *str ) {
  struct tm  ts;
  time_t rawtime = unix_ts;

  // printf( "ts=%.0f div/1000=%.0f\n" , node->valuedouble , node->valuedouble/1000 );
  ts = *localtime(&rawtime);
  // strftime( str, 25, "%a %Y-%m-%d %H:%M:%S %Z", &ts); creates eg, Mon 2022-05-05 00:00:16 , but end time always same, dont need day
  strftime( str, 11, "%Y-%m-%d", &ts);
  // printf("\t%s\n", str);

}

