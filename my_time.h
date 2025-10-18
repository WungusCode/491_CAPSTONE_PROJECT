#ifndef _MY_TIME_H

#define _MY_TIME_H

#define ONE_DAY_TS      86400

#define ONE_WEEK_TS    604800
#define TWO_WEEK_TS   1209600
#define FOUR_WEEK_TS  2419200

#define ONE_YEAR_TS  31536000

extern uint32_t get_unix_time_now( void );
extern int      get_month_now( void );
extern uint32_t get_month_start_ts( void );
extern uint32_t get_month_end_ts( void );

extern uint32_t get_n_month_start_ts( int Mth );
extern uint32_t get_n_month_end_ts  ( int Mth );

extern void convert_to_date_string( float unix_ts , char *str );
#endif
