// parses argv, prints out help file etc

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base_defs.h"
#include "data_types.h"

void print_help( void ) {
  printf( "  -h  = help\n" );
  printf( "  -d  = enable debug prints\n\n" );

}

int parse_cmdline(int argc, char *argv[] , papp_flags flgs )
{
  int ii = 0;
  int rc = 0;

  printf( "  >> E %s \n" , __FUNCTION__ );

  if ( argc > 1 ) {
    while ( ii < argc ) {
      if ( strcmp( "-h" , argv[ii] ) == 0 ) {
        print_help();
        rc = 1;
        goto owari;
      } // -h
      if ( strcmp( "-d" , argv[ii] ) == 0 ) {
        flgs->dbg = 1;
      }

      ii++;
    } // while
  } // if
owari:
  printf( "  << Lv %s \n" , __FUNCTION__ );

  return rc;
}

