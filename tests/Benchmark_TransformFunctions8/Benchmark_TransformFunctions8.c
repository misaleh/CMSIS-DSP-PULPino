#include "riscv_math.h"
#include "gpio.h" //for indication for benchmarking
#include "utils.h"
#include "string_lib.h"
#include "bar.h"
#include <stdio.h>
#include "riscv_const_structs.h"
#include "bench.h"

#define EVENT_ID 0x00  /*number of cycles ID for benchmarking*/

#define PRINT_F32(X,Y) printf("\n"); for(int i =0 ; i < (Y); i++) printf("%d  ",(int)(X[i]*100)); \
printf("\n\n")
#define PRINT_Q(X,Y) printf("\n"); for(int i =0 ; i < (Y); i++) printf("0x%X  ",X[i]); \
printf("\n\n")
//#define PRINT_OUTPUT  /*for testing functionality for each function, removed while benchmarking*/
#define RFFT_LEN  128
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


/*
*Each function has a GPIO pin (5 and 6 alternatively ) set before it runs and is cleared after it finish running
to measure the time of execution of each function.
*Define PRINT_OUTPUT to print the results to check for the functionality of the functions(may be slow)
*Also the correct results are printed for the current values which are calculated from the orignal library 
and also were checked by hand
*/
void perf_enable_id( int eventid){
  cpu_perf_conf_events(SPR_PCER_EVENT_MASK(eventid));
  cpu_perf_conf(SPR_PCMR_ACTIVE | SPR_PCMR_SATURATE);
};


q31_t testInput_q31[RFFT_LEN] = 
{   
0x4212, 	 	0xA973, 	      0xDA07, 	 	0x3030, 	 	
0x557E, 	 	0xFF00,               0x31B2, 	 	0x5643, 	
0xBD07, 		0x66E2, 	 	0xA110, 		0xB005, 		
0x8812, 	 	0x3127, 	 	0x6227, 	 	0x724A, 
0x4212, 	 	0xA973, 	      0xDA07, 	 	0x3030, 	 	
0x557E, 	 	0xFF00,               0x31B2, 	 	0x5643, 	
0xBD07, 		0x66E2, 	 	0xA110, 		0xB005, 		
0x8812, 	 	0x3127, 	 	0x6227, 	 	0x724A, 	
0x4212, 	 	0xA973, 	      0xDA07, 	 	0x3030, 	 	
0x557E, 	 	0xFF00,               0x31B2, 	 	0x5643, 	
0xBD07, 		0x66E2, 	 	0xA110, 		0xB005, 		
0x8812, 	 	0x3127, 	 	0x6227, 	 	0x724A, 
0x4212, 	 	0xA973, 	      0xDA07, 	 	0x3030, 	 	
0x557E, 	 	0xFF00,               0x31B2, 	 	0x5643, 	
0xBD07, 		0x66E2, 	 	0xA110, 		0xB005, 		
0x8812, 	 	0x3127, 	 	0x6227, 	 	0x724A,   
0x4212, 	 	0xA973, 	      0xDA07, 	 	0x3030, 	 	
0x557E, 	 	0xFF00,               0x31B2, 	 	0x5643, 	
0xBD07, 		0x66E2, 	 	0xA110, 		0xB005, 		
0x8812, 	 	0x3127, 	 	0x6227, 	 	0x724A, 
0x4212, 	 	0xA973, 	      0xDA07, 	 	0x3030, 	 	
0x557E, 	 	0xFF00,               0x31B2, 	 	0x5643, 	
0xBD07, 		0x66E2, 	 	0xA110, 		0xB005, 		
0x8812, 	 	0x3127, 	 	0x6227, 	 	0x724A, 	
0x4212, 	 	0xA973, 	      0xDA07, 	 	0x3030, 	 	
0x557E, 	 	0xFF00,               0x31B2, 	 	0x5643, 	
0xBD07, 		0x66E2, 	 	0xA110, 		0xB005, 		
0x8812, 	 	0x3127, 	 	0x6227, 	 	0x724A, 
0x4212, 	 	0xA973, 	      0xDA07, 	 	0x3030, 	 	
0x557E, 	 	0xFF00,               0x31B2, 	 	0x5643, 	
0xBD07, 		0x66E2, 	 	0xA110, 		0xB005, 		
0x8812, 	 	0x3127, 	 	0x6227, 	 	0x724A,  	
};

uint32_t ifftFlag = 0;
uint32_t doBitReverse = 0;

riscv_rfft_instance_q31 S_rfft_q31;
riscv_cfft_radix4_instance_q31  S_cfft_q31;
riscv_dct4_instance_q31 S_dct_q31;

q31_t state_q31[4*RFFT_LEN] = {0};  
riscv_status status;
int32_t main(void)
{
/*Init*/ 
/*dct Init*/
  status = riscv_cfft_radix4_init_q31(&S_cfft_q31,RFFT_LEN/2,ifftFlag,doBitReverse);
  printf("status = %d\n",status);
  status = riscv_rfft_init_q31(&S_rfft_q31,RFFT_LEN,ifftFlag,doBitReverse); 	
  printf("status = %d\n",status);
  status = riscv_dct4_init_q31(&S_dct_q31, &S_rfft_q31, &S_cfft_q31, RFFT_LEN,RFFT_LEN/2, 0x10000000);/*0.125 in q31 format*/
  printf("status = %d\n",status);
/*Tests*/
/*dct*/
  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_dct4_q31(&S_dct_q31,state_q31, testInput_q31);
  perf_stop();
  printf("riscv_dct4_q31: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  PRINT_Q(testInput_q31,RFFT_LEN);
#endif
  printf("End\n");


 return 0;
}



