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


float32_t testInput_f32[RFFT_LEN] = 
{   
-0.865129623056441,  	       -2.655020678073846, 	 	0.600664612949661, 	 	0.080378093886515, 	 	
-2.899160484012034, 	 	2.563004262857762, 	 	3.078328403304206, 	 	0.105906778385130, 	 
-0.865129623056441,  	       -2.655020678073846, 	 	0.600664612949661, 	 	0.080378093886515, 	 	
-2.899160484012034, 	 	2.563004262857762, 	 	3.078328403304206, 	 	0.105906778385130, 
-0.865129623056441,  	       -2.655020678073846, 	 	0.600664612949661, 	 	0.080378093886515, 	 	
-2.899160484012034, 	 	2.563004262857762, 	 	3.078328403304206, 	 	0.105906778385130, 	 
-0.865129623056441,  	       -2.655020678073846, 	 	0.600664612949661, 	 	0.080378093886515, 	 	
-2.899160484012034, 	 	2.563004262857762, 	 	3.078328403304206, 	 	0.105906778385130, 	
-0.865129623056441,  	       -2.655020678073846, 	 	0.600664612949661, 	 	0.080378093886515, 	 	
-2.899160484012034, 	 	2.563004262857762, 	 	3.078328403304206, 	 	0.105906778385130, 	 
-0.865129623056441,  	       -2.655020678073846, 	 	0.600664612949661, 	 	0.080378093886515, 	 	
-2.899160484012034, 	 	2.563004262857762, 	 	3.078328403304206, 	 	0.105906778385130, 
-0.865129623056441,  	       -2.655020678073846, 	 	0.600664612949661, 	 	0.080378093886515, 	 	
-2.899160484012034, 	 	2.563004262857762, 	 	3.078328403304206, 	 	0.105906778385130, 	 
-0.865129623056441,  	       -2.655020678073846, 	 	0.600664612949661, 	 	0.080378093886515, 	 	
-2.899160484012034, 	 	2.563004262857762, 	 	3.078328403304206, 	 	0.105906778385130, 
-0.865129623056441,  	       -2.655020678073846, 	 	0.600664612949661, 	 	0.080378093886515, 	 	
-2.899160484012034, 	 	2.563004262857762, 	 	3.078328403304206, 	 	0.105906778385130, 	 
-0.865129623056441,  	       -2.655020678073846, 	 	0.600664612949661, 	 	0.080378093886515, 	 	
-2.899160484012034, 	 	2.563004262857762, 	 	3.078328403304206, 	 	0.105906778385130, 
-0.865129623056441,  	       -2.655020678073846, 	 	0.600664612949661, 	 	0.080378093886515, 	 	
-2.899160484012034, 	 	2.563004262857762, 	 	3.078328403304206, 	 	0.105906778385130, 	 
-0.865129623056441,  	       -2.655020678073846, 	 	0.600664612949661, 	 	0.080378093886515, 	 	
-2.899160484012034, 	 	2.563004262857762, 	 	3.078328403304206, 	 	0.105906778385130, 
-0.865129623056441,  	       -2.655020678073846, 	 	0.600664612949661, 	 	0.080378093886515, 	 	
-2.899160484012034, 	 	2.563004262857762, 	 	3.078328403304206, 	 	0.105906778385130, 	 
-0.865129623056441,  	       -2.655020678073846, 	 	0.600664612949661, 	 	0.080378093886515, 	 	
-2.899160484012034, 	 	2.563004262857762, 	 	3.078328403304206, 	 	0.105906778385130, 
-0.865129623056441,  	       -2.655020678073846, 	 	0.600664612949661, 	 	0.080378093886515, 	 	
-2.899160484012034, 	 	2.563004262857762, 	 	3.078328403304206, 	 	0.105906778385130, 	 
-0.865129623056441,  	       -2.655020678073846, 	 	0.600664612949661, 	 	0.080378093886515, 	 	
-2.899160484012034, 	 	2.563004262857762, 	 	3.078328403304206, 	 	0.105906778385130,  		 
};


uint32_t ifftFlag = 0;
uint32_t doBitReverse = 0;

riscv_rfft_instance_f32 S_rfft_f32;
riscv_cfft_radix4_instance_f32  S_cfft_f32;
riscv_dct4_instance_f32 S_dct_f32;

float32_t state_f32[4*RFFT_LEN] = {0};  

int32_t main(void)
{
/*Init*/ 
/*rfft Init*/
  riscv_cfft_radix4_init_f32(&S_cfft_f32,RFFT_LEN/2,ifftFlag,doBitReverse);
  riscv_rfft_init_f32(&S_rfft_f32,&S_cfft_f32,RFFT_LEN,ifftFlag,doBitReverse); 	
  riscv_dct4_init_f32(&S_dct_f32, &S_rfft_f32, &S_cfft_f32, RFFT_LEN,RFFT_LEN/2, 0.125);
/*Tests*/
/*dct*/
  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_dct4_f32(&S_dct_f32,state_f32, testInput_f32);
  perf_stop();
  printf("riscv_dct4_f32: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  PRINT_F32(testInput_f32,RFFT_LEN);
#endif
  printf("End\n");

 return 0;
}



