#include <stdio.h>
#include <mips.h>
#include <time.h>

int main(int argc, char **argv) {

  printf("AND Driver\n");
  andgate_driver(true, true);
  andgate_driver(true, false);
  andgate_driver(false, true);
  andgate_driver(false, false);

  printf("OR Driver\n");
  orgate_driver(true, true);
  orgate_driver(true, false);
  orgate_driver(false, true);
  orgate_driver(false, false);

  printf("NOT Driver\n");
  notgate_driver(true);
  notgate_driver(false);

  printf("NAND Driver\n");
  nandcircuit_driver(true, true);
  nandcircuit_driver(true, false);
  nandcircuit_driver(false, true);
  nandcircuit_driver(false, false);

  printf("XOR Driver\n");
  xorcircuit_driver(true, true);
  xorcircuit_driver(true, false);
  xorcircuit_driver(false, true);
  xorcircuit_driver(false, false);

  printf("FA Driver\n");
  fa_driver(true, true, true);
  fa_driver(true, true, false);
  fa_driver(true, false, true);
  fa_driver(true, false, false);
  fa_driver(false, true, true);
  fa_driver(false, true, false);
  fa_driver(false, false, true);
  fa_driver(false, false, false);
  
  printf("RCA Driver\n");
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

  printf("CLA4 Driver\n");
  cla4_driver(2, 3);
  cla4_driver(0, 0);
  cla4_driver(1, 0);
  cla4_driver(0, 1);
  cla4_driver(4, 11);
  cla4_driver(4, 12);

  /*
  printf("MUX Driver\n");
  mux_driver(false, true, false);
  mux_driver(false, false, true);
  mux_driver(true, false, true);
  mux_driver(true, true, false);

  printf("MUX4 Driver\n");
  mux4_driver(false, false, true, false, false, false);
  mux4_driver(true, false, false, true, false, false);
  mux4_driver(true, false, true, false, true, true);
  mux4_driver(false, true, false, false, true, false);
  mux4_driver(true, true, false, false, false, true);

  printf("ALU Driver\n");
  printf("//and test\n");     
  alu_driver(1, 0, 0, 0, 0, 0);      
  alu_driver(1, 1, 0, 0, 0, 0);   
  printf("//or test\n");      
  alu_driver(1, 0, 1, 0, 0, 0);         
  printf("//addition\n");     
  alu_driver(1, 0, 0, 1, 0, 0);
  alu_driver(0, 1, 0, 1, 0, 0);
  alu_driver(1, 1, 0, 1, 0, 0);
  printf("//substraction\n"); 
  alu_driver(1, 1, 0, 1, 1, 1);
  alu_driver(1, 0, 0, 1, 1, 1);
  alu_driver(0, 0, 0, 1, 1, 1);
  
  printf("ALU_MSB Driver\n");
  printf("//and test\n");       
  alu_msb_driver(1, 0, 0, 0, 0, 0); 
  alu_msb_driver(1, 1, 0, 0, 0, 0);        
  printf("//or test\n");        
  alu_msb_driver(1, 0, 1, 0, 0, 0);         
  printf("//addition\n");       
  alu_msb_driver(1, 0, 0, 1, 0, 0);
  alu_msb_driver(0, 1, 0, 1, 0, 0);
  alu_msb_driver(1, 1, 0, 1, 0, 0);
  printf("//substraction\n");   
  alu_msb_driver(1, 1, 0, 1, 1, 1);
  alu_msb_driver(1, 0, 0, 1, 1, 1);
  alu_msb_driver(0, 0, 0, 1, 1, 1);
  */

  printf("ALU32 Driver\n");
  printf("//and test\n");     
  alu32_driver(12, 4, 0, 0, 0);   
  alu32_driver(0, 0, 0, 0, 0);
  printf("//or test\n");      
  alu32_driver(10, 4, 1, 0, 0);         
  printf("//addition\n");     
  alu32_driver(12, 4, 0, 1, 0);
  printf("//substraction\n"); 
  alu32_driver(1384, 324, 0, 1, 1);
  alu32_driver(323, 1384, 0, 1, 1);
  printf("//slt\n");
  alu32_driver(2, 4, 1, 1, 1);
  alu32_driver(4, 2, 1, 1, 1);
  printf("//overflow\n");
  alu32_driver(2147483647, 10, 0, 1, 0);
  alu32_driver(-2147483648, -10, 0, 1, 0);
  alu32_driver(2147483647, -10, 0, 1, 1);
  alu32_driver(-2147483648, 10, 0, 1, 1);

  /*  
  printf("ALU32 Driver\n");
  printf("and test\n");     
  alu32_driver(1, 1, 0, 0, 0);  
  printf("or test\n");      
  alu32_driver(10, 4, 1, 0, 0);         
  printf("addition\n");     
  alu32_driver(12, 4, 0, 1, 0);
  printf("substraction\n"); 
  alu32_driver(12, 4, 0, 1, 1);
  printf("slt\n");          
  alu32_driver(12, 4, 1, 1, 1);
  */

  return 0;
}
