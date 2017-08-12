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
  set_pin_function(5, FUNC_GPIO);
  set_gpio_pin_direction(5, DIR_OUT);
  set_pin_function(6, FUNC_GPIO);
  set_gpio_pin_direction(6, DIR_OUT);
  CLR_GPIO_5();
  CLR_GPIO_6();
  /*rfft Init*/
  riscv_cfft_radix4_init_f32(&S_cfft_f32,RFFT_LEN/2,ifftFlag,doBitReverse);
  riscv_rfft_init_f32(&S_rfft_f32,&S_cfft_f32,RFFT_LEN,ifftFlag,doBitReverse); 	
  riscv_dct4_init_f32(&S_dct_f32, &S_rfft_f32, &S_cfft_f32, RFFT_LEN,RFFT_LEN/2, 0.125);
/*Tests*/
/*dct*/
  SET_GPIO_6();	
  riscv_dct4_f32(&S_dct_f32,state_f32, testInput_f32);
  CLR_GPIO_6();	
#ifdef PRINT_OUTPUT
  PRINT_F32("riscv_dct4_f32",testInput_f32,RFFT_LEN);
#endif
  printf("End\n");

 return 0;
}



