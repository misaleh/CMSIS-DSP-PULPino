#include "riscv_math.h"
#include "gpio.h" //for indication for benchmarking
#include "utils.h"
#include "string_lib.h"
#include "bar.h"
#include <stdio.h>
#include "riscv_const_structs.h"
#define PRINT_F32(Z,X,Y) printf(Z"\n"); for(int i =0 ; i < (Y); i++) printf("%d  ",(int)(X[i]*100)); \
printf("\n\n")
#define PRINT_Q(Z,X,Y) printf(Z"\n"); for(int i =0 ; i < (Y); i++) printf("0x%X  ",X[i]); \
printf("\n\n")
#define PRINT_OUTPUT  /*for testing functionality for each function, removed while benchmarking*/
#define TEST_LENGTH_SAMPLES 128
#define RFFT_LEN  64
/*
  this macros used for benchmarking, they are not friendly as they affect other GPIOs, but the main purpose here is to minimize the overhead,
a more fierndly version will be
#define SET_GPIO_6()     *(volatile int*) (GPIO_REG_PADOUT) |=  0x20;
#define CLR_GPIO_6()    *(volatile int*) (GPIO_REG_PADOUT) &=  0xDF;
#define SET_GPIO_5()     *(volatile int*) (GPIO_REG_PADOUT) |=  0x10;
#define CLR_GPIO_5()     *(volatile int*) (GPIO_REG_PADOUT) &=  0xEF;
*/

#define SET_GPIO_6()     *(volatile int*) (GPIO_REG_PADOUT) =  0x20;
#define CLR_GPIO_6()    *(volatile int*) (GPIO_REG_PADOUT) =  0x00;
#define SET_GPIO_5()     *(volatile int*) (GPIO_REG_PADOUT) =  0x10;
#define CLR_GPIO_5() *(volatile int*) (GPIO_REG_PADOUT) = 0x00;
//#define PRINT_OUTPUT

/*
*Each function has a GPIO pin (5 and 6 alternatively ) set before it runs and is cleared after it finish running
to measure the time of execution of each function.
*Define PRINT_OUTPUT to print the results to check for the functionality of the functions(may be slow)
*Also the correct results are printed for the current values which are calculated from the orignal library 
and also were checked by hand
*/


q31_t testInput_q31[TEST_LENGTH_SAMPLES] = 
{   
0x42120000, 	0, 	0xA9730000, 	0,      0xDA070000, 	0, 	0x30300000, 	0, 	
0x557E0000, 	0, 	0xFF000000, 	0, 	0x31B20000, 	0, 	0x56430000, 	0, 	
0xBD070000, 	0, 	0x66E20000, 	0, 	0xA1100000, 	0, 	0xB0050000, 	0, 	
0x88120000, 	0, 	0x31270000, 	0, 	0x62270000, 	0, 	0x724A0000, 	0, 	
0x19820000, 	0, 	0x456A0000, 	0, 	0x00170000, 	0, 	0xC90C0000,  	0, 	
0xE0000000, 	0, 	0x221C0000, 	0, 	0x10440000, 	0, 	0x27050000, 	0, 	
0x13800000, 	0, 	0x2F980000, 	0, 	0x41900000, 	0, 	0xF1110000, 	0, 	
0xBDDB0000, 	0, 	0x10000000, 	0, 	0x00990000, 	0, 	0x74420000, 	0, 	
0xA1F10000, 	0, 	0x31890000, 	0, 	0x69870000, 	0, 	0x50500000, 	0, 	
0x5A420000, 	0,      0x31090000, 	0, 	0x22220000, 	0, 	0x18150000, 	0, 	
0x34A80000, 	0, 	0x33340000, 	0, 	0x54230000, 	0, 	0x69710000, 	0, 	
0x77790000, 	0, 	0xA1110000, 	0, 	0x82970000, 	0, 	0x51190000, 	0, 	
0x10100000, 	0, 	0x43A90000, 	0,      0x55AA0000, 	0, 	0x00100000, 	0, 	
0x40390000, 	0, 	0x00200000, 	0, 	0x30010000, 	0, 	0x00100000, 	0, 	
0x15230000, 	0, 	0x30140000, 	0, 	0x97970000, 	0, 	0x57970000, 	0, 	
0x00080000, 	0, 	0x110C0000, 	0, 	0x3A420000, 	0, 	0xA2120000, 	0, 
};

uint32_t fftSize = 64;
uint32_t ifftFlag = 0;
uint32_t doBitReverse = 0;
riscv_rfft_fast_instance_f32 S_rfft_f32;


int32_t main(void)
{
 /*Init*/ 
  set_pin_function(5, FUNC_GPIO);
  set_gpio_pin_direction(5, DIR_OUT);
  set_pin_function(6, FUNC_GPIO);
  set_gpio_pin_direction(6, DIR_OUT);
  CLR_GPIO_5();
  CLR_GPIO_6();

/*Tests*/

/*cfft*/
  printf("bitrev =%d\n",doBitReverse);
  SET_GPIO_5();	
  riscv_cfft_q31(&riscv_cfft_sR_q31_len64, testInput_q31, ifftFlag, doBitReverse);
  CLR_GPIO_5();	
#ifdef PRINT_OUTPUT
  PRINT_Q("riscv_cfft_q31",testInput_q31,fftSize);
#endif

  printf("End\n");

 return 0;
}



