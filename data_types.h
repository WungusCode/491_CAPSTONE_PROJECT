#ifndef _DATA_TYPES_H

#define _DATA_TYPES_H 1

#include <stdint.h>

typedef struct _app_flags {
  int dbg;
  int autoTest;
} app_flags, * papp_flags;

#endif
