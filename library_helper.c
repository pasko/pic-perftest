/* TODO: put license here. */

#include <stdio.h>

int __attribute__ ((visibility ("default"))) gLast;

int Helper(int i) {
  gLast = i + 5;
  return gLast;
}
