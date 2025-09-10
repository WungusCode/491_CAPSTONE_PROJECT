#include <stdio.h>
#include<gtk/gtk.h>

#include "base_defs.h"
#include "data_types.h"

#include "params.h"

static app_flags flgs;

int main(int argc, char* argv[]) {
  int rc = 0;

  rc = parse_cmdline( argc, argv , &flgs );
  if ( rc == 0 ) {
    gtk_init(&argc, &argv);

    // code

    gtk_main();  // blocks until GTK terminates
  }
  return rc;
}
