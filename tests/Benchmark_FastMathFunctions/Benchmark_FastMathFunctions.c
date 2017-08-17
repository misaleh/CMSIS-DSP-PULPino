#include <math.h>
#include <stdio.h>
#include "riscv_math.h"
#include "gpio.h" //for indication for benchmarking
#include "utils.h"
#include "string_lib.h"
#include "bar.h"
#include "bench.h"
#define EVENT_ID 0x00  /*number of cycles ID for benchmarking*/
//#define PRINT_OUTPUT  /*for testing functionality for each function, removed while benchmarking*/
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
#define CLR_GPIO_5()     *(volatile int*) (GPIO_REG_PADOUT) =  0x00;

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
/* ouput variables*/
float32_t result_f32;  
q15_t result_q15;
q31_t result_q31;
int i = 0 ;
q15_t angle_q15 = 0x7FFF ; /*360*/
q31_t angle_q31 = 0x7FFFFFFF ; /*360*/
int angle =360;
volatile float32_t test_f= 4.2;
volatile q15_t test_q15 = 0x1245;
volatile q31_t test_q31 = 0x48A3617B;
volatile float32_t test_angle_f32 = 0.9;
volatile q15_t test_angle_q15 = 0x0FD6;
volatile q31_t test_angle_q31 = 0x07FFFFFF;
int32_t main(void)
{

/*Tests*/
/*sqrt*/
  test_f +=2.0;
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_sqrt_f32(test_f,&result_f32);
  perf_stop();
  printf("riscv_sqrt_f32: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  printf("%d ",(int)(result_f32*100));  
  printf("\n");
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_sqrt_q15(test_q15,&result_q15);
  perf_stop();
  printf("riscv_sqrt_q15: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  printf("0x%X ",result_q15);  
  printf("\n");
  printf("Correct answer = 0x305B \n");
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_sqrt_q31(test_q31,&result_q31);
  perf_stop();
  printf("riscv_sqrt_q31: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  printf("0x%X ",result_q31);  
  printf("\n");
  printf("Correct answer = 0x606CAE24 \n");
#endif

/*cos*/

  perf_reset();
  perf_enable_id(EVENT_ID);	
  result_f32=riscv_cos_f32(test_angle_f32);
  perf_stop();
  printf("riscv_cos_f32: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  printf("%d ",(int)(result_f32*100));  
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  result_q15=riscv_cos_q15(test_angle_q15);
  perf_stop();
  printf("riscv_cos_q15: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  printf("0x%X ",result_q15);  
  printf("\n");
  printf("Correct answer = 0x5B3A \n");
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);
  result_q31=riscv_cos_q31(test_angle_q31);
  perf_stop();
  printf("riscv_cos_q31: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  printf("0x%X ",result_q31); 
  printf("\n");
  printf("Correct answer = 0x7641AF3C \n"); 
#endif

/*sin*/

  perf_reset();
  perf_enable_id(EVENT_ID);	
  result_f32=riscv_sin_f32(test_angle_f32);
  perf_stop();
  printf("riscv_sin_f32: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  printf("%d ",(int)(result_f32*100));  
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  result_q15=riscv_sin_q15(test_angle_q15);
  perf_stop();
  printf("riscv_sin_q15: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  printf("0x%X ",result_q15); 
  printf("\n");
  printf("Correct answer = 0x59C4 \n"); 
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);
  result_q31=riscv_sin_q31(test_angle_q31);
  perf_stop();
  printf("riscv_sin_q31: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  printf("0x%X ",result_q31);  
  printf("\n");
  printf("Correct answer = 0x30FBC546 \n");
#endif

/*More tests*/
/*
angle_q15 = 0x7FFF ;
printf("riscv_cos_q15\n"); 
for(int i = 0 ; i < 10  ; i++)
{
  result_q15=riscv_cos_q15(angle_q15);
  angle_q15 = angle_q15 >> 1; //divide by 2
  printf("Angle = %d,Fixed =  %d, result in fixed point is %d, equivalent(*100) = %d\n",angle,angle_q15,result_q15,(result_q15/327));
  angle = angle /2;
}


angle = 360;
angle_q31 = 0x7FFFFFFF ; 
printf("riscv_cos_q31\n"); 

for(int i = 0 ; i < 10 ; i++)
{
  result_q31=riscv_cos_q31(angle_q31);
  angle_q31 = angle_q31 >> 1; //divide by 2
  printf("Angle = %d,Fixed =  %d, result in fixed point is %d, equivalent(*100) = %d\n",angle,angle_q31,result_q31,(result_q31/21474836));
  angle = angle /2;
}

angle = 360;
angle_q15 = 0x7FFF ;
printf("riscv_sin_q15\n"); 
//input from range 0 to 0.999, scaled to [0 2*pi]
for(int i = 0 ; i < 10 ; i++)
{
  result_q15=riscv_sin_q15(angle_q15);
  angle_q15 = angle_q15 >> 1; //divide by 2
  printf("Angle = %d,Fixed =  %d, result in fixed point is %d, equivalent(*100) = %d\n",angle,angle_q15,result_q15,(result_q15/327));
  angle = angle /2;
}


angle = 360;
angle_q31 = 0x7FFFFFFF ; //360
printf("riscv_sin_q31\n"); 
//input from range 0 to 0.999, scaled to [0 2*pi]
for(int i = 0 ; i < 10 ; i++)
{
  result_q31=riscv_sin_q31(angle_q31);
  angle_q31 = angle_q31 >> 1; //divide by 2
  printf("Angle = %d,Fixed =  %d, result in fixed point is %d, equivalent(*100) = %d\n",(int)angle,(q31_t)angle_q31,(q31_t)result_q31,(q31_t)(result_q31/21474836));
  angle = angle /2;
}
*/


  printf("End\n");
  return 0 ;
}
