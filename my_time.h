#ifndef _MY_TIME_H

#define _MY_TIME_H

#define ONE_YEAR_TS 31536000

extern uint32_t get_unix_time_now( void );

extern void convert_to_date_string( float unix_ts , char *str );
#endif
