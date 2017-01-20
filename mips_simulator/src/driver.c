#include <stdio.h>
#include <mips.h>
#include <time.h>

int main(int argc, char **argv) {

  printf("ANDDriver\n");
  andgate_driver(true, true);
  andgate_driver(true, false);
  andgate_driver(false, true);
  andgate_driver(false, false);

  printf("ORDriver\n");
  orgate_driver(true, true);
  orgate_driver(true, false);
  orgate_driver(false, true);
  orgate_driver(false, false);

  printf("NOTDriver\n");
  notgate_driver(true);
  notgate_driver(false);

  printf("NANDDriver\n");
  nandcircuit_driver(true, true);
  nandcircuit_driver(true, false);
  nandcircuit_driver(false, true);
  nandcircuit_driver(false, false);

  printf("XORDriver\n");
  xorcircuit_driver(true, true);
  xorcircuit_driver(true, false);
  xorcircuit_driver(false, true);
  xorcircuit_driver(false, false);

  printf("FADriver\n");
  fa_driver(true, true, true);
  fa_driver(true, true, false);
  fa_driver(true, false, true);
  fa_driver(true, false, false);
  fa_driver(false, true, true);
  fa_driver(false, true, false);
  fa_driver(false, false, true);
  fa_driver(false, false, false);
  
  printf("RCADriver\n");
  rca_driver(0, 0);
  rca_driver(1, 0);
  rca_driver(0, 1);
  rca_driver(9823749, 2834234);
  rca_driver(7823644, -928374);
  
  /*
  printf("PFADriver\n");
  pfa_driver(true, true, true);
  pfa_driver(true, true, false);
  pfa_driver(true, false, true);
  pfa_driver(true, false, false);
  pfa_driver(false, true, true);
  pfa_driver(false, true, false);
  pfa_driver(false, false, true);
  pfa_driver(false, false, false);

  printf("CLUDriver\n");
  clu_driver(1, 14);
  clu_driver(5, 3);
  */

  printf("CLA4Driver\n");
  cla4_driver(2, 3);
  cla4_driver(0, 0);
  cla4_driver(1, 0);
  cla4_driver(0, 1);
  cla4_driver(4, 11);
  cla4_driver(4, 12);

  return 0;
}
