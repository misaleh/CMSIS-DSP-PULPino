#include <math.h>
#include <stdio.h>
#include "riscv_math.h"
#include "gpio.h"
#include "utils.h"
#include "string_lib.h"
#include "bar.h"
#include "bench.h"

#define EVENT_ID 0x00  /*number of cycles ID for benchmarking*/


//#define PRINT_OUTPUT  /*for testing functionality for each function, removed while benchmarking*/
#define MAX_BLOCKSIZE 32
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
*Also the correct results are printed for the current vectors which are calculated from the orignal library 
and also were checked by hand
*/
void perf_enable_id( int eventid){
  cpu_perf_conf_events(SPR_PCER_EVENT_MASK(eventid));
  cpu_perf_conf(SPR_PCMR_ACTIVE | SPR_PCMR_SATURATE);
};
 volatile float32_t srcA_buf_f32[MAX_BLOCKSIZE] =
{
  -0.4325648115282207,  -1.6655843782380970,  0.1253323064748307,
   0.2876764203585489,  -1.1464713506814637,  1.1909154656429988,
   1.1891642016521031,  -0.0376332765933176,  0.3272923614086541,
   0.1746391428209245,  -0.1867085776814394,  0.7257905482933027,
  -0.5883165430141887,   2.1831858181971011, -0.1363958830865957,
   0.1139313135208096,   1.0667682113591888,  0.0592814605236053,
  -0.0956484054836690,  -0.8323494636500225,  0.2944108163926404,
  -1.3361818579378040,   0.7143245518189522,  1.6235620644462707,
  -0.6917757017022868,   0.8579966728282626,  1.2540014216025324,
  -1.5937295764474768,  -1.4409644319010200,  0.5711476236581780,
  -0.3998855777153632,   0.6899973754643451
};

 volatile q7_t srcA_buf_q7[MAX_BLOCKSIZE] =
{
   0x75,  0x13,   0x15,
   0x44,  0x11,   0x1C,
   0x52,  0x0B,   0x83,
   0x91,  0x33,   0x25,
   0x1A,  0x4D,   0x6F,
   0x26,  0x01,   0xF1,
   0xAC,  0x66,   0x76,
   0x54,  0x87,   0x36,
   0x22,  0x33,   0xE1,
   0x61,  0x54,   0x35,
   0x91,  0x49
};
 volatile q15_t srcA_buf_q15[MAX_BLOCKSIZE] =
{
   0x7512,  0x1375,   0x1565,
   0x44C3,  0x1188,   0x1CA1,
   0x5264,  0x0B20,   0x8305,
   0x9112,  0x3399,   0x2518,
   0x1AB2,  0x4D01,   0x6F23,
   0x26FF,  0x0121,   0xF123,
   0xAC47,  0x6688,   0x76A2,
   0x5476,  0x8756,   0x36B3,
   0x2245,  0x3373,   0xE1A9,
   0x610A,  0x5419,   0x3501,
   0x9100,  0x4469
};

 volatile q31_t srcA_buf_q31[MAX_BLOCKSIZE] =
{
   0x75122542,  0x1375A138,   0x15654473,
   0x44C34463,  0x118854C3,   0x1CA18291,
   0x5264E236,  0x0B200590,   0x83059A77,
   0x91129713,  0x3399D123,   0x2518BBD1,
   0x1AB2037A,  0x4D01D1F1,   0x6F237531,
   0x26FF5569,  0x01213159,   0xF123CD77,
   0xAC475456,  0x6688A12E,   0x76A2FF96,
   0x54761269,  0x8756D438,   0x36B3B697,
   0x22459643,  0x3373C368,   0xE1A901D3,
   0x610A0540,  0x54199643,   0x350112B5,
   0x91000436,  0x4469AA15
};




float32_t result_f32; 
q7_t result_q7;
q15_t result_q15;
q31_t result_q31;
volatile float32_t X_f32 = 1.3;
volatile int32_t X_Q = 0xA00000;

int32_t main(void)
{

 /*Init*/ 

  volatile riscv_linear_interp_instance_f32 S_linear_f32 = {MAX_BLOCKSIZE, 2.3, 0.1, srcA_buf_f32};
  volatile riscv_bilinear_interp_instance_f32 S_bilinear_f32= {5,5,srcA_buf_f32};/*only 25 elements from table*/
  volatile riscv_bilinear_interp_instance_q7 S_bilinear_q7= {5,5,srcA_buf_q7};/*only 25 elements from table*/
  volatile riscv_bilinear_interp_instance_q15 S_bilinear_q15= {5,5,srcA_buf_q15};/*only 25 elements from table*/
  volatile riscv_bilinear_interp_instance_q31 S_bilinear_q31= {5,5,srcA_buf_q31};/*only 25 elements from table*/
/*Tests*/
/*Linear Interpolation*/

  perf_reset();
  perf_enable_id(EVENT_ID);	
  result_f32 = riscv_linear_interp_f32(&S_linear_f32, X_f32);
  perf_stop();
  printf("riscv_linear_interp_f32: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  printf(" %d\n",(int)(100*result_f32));
#endif	

  perf_reset();
  perf_enable_id(EVENT_ID);	
  result_q7 = riscv_linear_interp_q7(srcA_buf_q7,X_Q,MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_linear_interp_q7: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  printf(" 0x%X\n",result_q7);
#endif

  perf_reset();
  perf_enable_id(EVENT_ID);	
  result_q15 = riscv_linear_interp_q15(srcA_buf_q15,X_Q,MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_linear_interp_q15: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  printf(" 0x%X\n",result_q15);
#endif

  perf_reset();
  perf_enable_id(EVENT_ID);	
  result_q31 = riscv_linear_interp_q31(srcA_buf_q31,X_Q,MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_linear_interp_q31: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  printf(" 0x%X\n",result_q31);
#endif	
/*Bilinear Interpolation*/
  perf_reset();
  perf_enable_id(EVENT_ID);	
  result_f32 = riscv_bilinear_interp_f32(&S_bilinear_f32, 3,2);
  perf_stop();
  printf("riscv_bilinear_interp_f32: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  printf(" %d\n",(int)(100*result_f32));
#endif	

  perf_reset();
  perf_enable_id(EVENT_ID);	
  result_q7 = riscv_bilinear_interp_q7(&S_bilinear_q7,3,2);
  perf_stop();
  printf("riscv_bilinear_interp_q7: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  printf("  0x%X\n",result_q7);
#endif

  perf_reset();
  perf_enable_id(EVENT_ID);	
  result_q15 = riscv_bilinear_interp_q15(&S_bilinear_q15,3,2);
  perf_stop();
  printf("riscv_bilinear_interp_q15: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  printf(" 0x%X\n",result_q15);
#endif

  perf_reset();
  perf_enable_id(EVENT_ID);	
  result_q31 = riscv_bilinear_interp_q31(&S_bilinear_q31,3,2);
  perf_stop();
  printf("riscv_bilinear_interp_q31: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  printf(" 0x%X\n",result_q31);
#endif
  printf("End\n");

  return 0 ;
}
