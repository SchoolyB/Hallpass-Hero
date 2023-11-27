#include <stdio.h>
#include "Cuazar.h"

int Init_Cuazar(void)
{
  testModeIsOn = TRUE;
  printf(GREEN "Cuazar initialized...\n" RESET);
  return 0;
}
