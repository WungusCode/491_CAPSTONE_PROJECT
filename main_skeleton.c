#include <stdio.h>
#include<gtk/gtk.h>

#include "base_defs.h"

int main(int argc, char* argv[]) {
  int rc = 0;

  gtk_init(&argc, &argv);

  // code

  gtk_main();  // blocks until GTK terminates
  return rc;
}
