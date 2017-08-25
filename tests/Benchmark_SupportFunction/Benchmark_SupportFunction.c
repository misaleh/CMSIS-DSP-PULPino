#include <stdio.h>
#include "riscv_math.h"
#include "gpio.h" //for indication for benchmarking
#include "utils.h"
#include "string_lib.h"
#include "bar.h"
#include "bench.h"

#define EVENT_ID 0x00  /*number of cycles ID for benchmarking*/


#define PRINT_F32(X,Y) printf("\n"); for(int i =0 ; i < Y; i++) printf("%d  ",(int)(X[i])); \
printf("\n\n")
#define PRINT_Q(X,Y) printf("\n"); for(int i =0 ; i < Y; i++) printf("0x%X  ",X[i]); \
printf("\n\n")
#define PRINT_OUTPUT  /*for testing functionality for each function, removed while benchmarking*/
#define MAX_BLOCKSIZE     32

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
 float32_t src_buf_f32[MAX_BLOCKSIZE] =
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

 q7_t src_buf_q7[MAX_BLOCKSIZE] =
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

 q15_t src_buf_q15[MAX_BLOCKSIZE] =
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

 q31_t src_buf_q31[MAX_BLOCKSIZE] =
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


float32_t val_f32 = 5.8;
q7_t val_q7 = 0x11;
q15_t val_q15= 0x51C1;
q31_t val_q31 = 0xA1A1DE53;

/* Final ouput */
float32_t result_f32[MAX_BLOCKSIZE] = {0};  
q7_t result_q7[MAX_BLOCKSIZE] = {0};
q15_t result_q15[MAX_BLOCKSIZE] = {0};
q31_t result_q31[MAX_BLOCKSIZE] = {0};


int32_t main(void)
{
 /*Init*/ 
/*Tests*/

/*Copy*/

  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_copy_f32(src_buf_f32,result_f32, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_copy_f32: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));	
#ifdef PRINT_OUTPUT
  PRINT_F32(result_f32,MAX_BLOCKSIZE);
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_copy_q7(src_buf_q7,result_q7, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_copy_q7: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));	
#ifdef PRINT_OUTPUT
  PRINT_Q(result_q7,MAX_BLOCKSIZE);
#endif  
  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_copy_q15(src_buf_q15,result_q15, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_copy_q15: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));	
#ifdef PRINT_OUTPUT
  PRINT_Q(result_q15,MAX_BLOCKSIZE);
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_copy_q31(src_buf_q31,result_q31, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_copy_q31: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));	
#ifdef PRINT_OUTPUT
  PRINT_Q(result_q31,MAX_BLOCKSIZE);
#endif

/*Fill*/

  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_fill_f32(val_f32,result_f32, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_fill_f32: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));	
#ifdef PRINT_OUTPUT
  PRINT_F32(result_f32,MAX_BLOCKSIZE);
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_fill_q7(val_q7,result_q7, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_fill_q7: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));	
#ifdef PRINT_OUTPUT
  PRINT_Q(result_q7,MAX_BLOCKSIZE);
#endif  
  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_fill_q15(val_q15,result_q15, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_fill_q15: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));	
#ifdef PRINT_OUTPUT
  PRINT_Q(result_q15,MAX_BLOCKSIZE);
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_fill_q31(val_q31,result_q31, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_fill_q31: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));	
#ifdef PRINT_OUTPUT
  PRINT_Q(result_q31,MAX_BLOCKSIZE);
#endif

/*convert float*/

  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_float_to_q7(src_buf_f32,result_q7, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_float_to_q7: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));	
#ifdef PRINT_OUTPUT
  PRINT_Q(result_q7,MAX_BLOCKSIZE);
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_float_to_q15(src_buf_f32,result_q15, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_float_to_q15: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));	
#ifdef PRINT_OUTPUT
  PRINT_Q(result_q15,MAX_BLOCKSIZE);
#endif  
  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_float_to_q31(src_buf_f32,result_q31, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_float_to_q31: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));	
#ifdef PRINT_OUTPUT
  PRINT_Q(result_q31,MAX_BLOCKSIZE);
#endif


/*convert q7*/

  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_q7_to_float(src_buf_q7,result_f32, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_q7_to_float: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));	
#ifdef PRINT_OUTPUT
  PRINT_F32(result_f32,MAX_BLOCKSIZE);
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_q7_to_q15(src_buf_q7,result_q15, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_q7_to_q15: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));	
#ifdef PRINT_OUTPUT
  PRINT_Q(result_q15,MAX_BLOCKSIZE);
#endif  
  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_q7_to_q31(src_buf_q7,result_q31, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_q7_to_q31: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));	
#ifdef PRINT_OUTPUT
  PRINT_Q(result_q31,MAX_BLOCKSIZE);
#endif


/*convert q15*/

  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_q15_to_float(src_buf_q15,result_f32, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_q15_to_float: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));	
#ifdef PRINT_OUTPUT
  PRINT_F32(result_f32,MAX_BLOCKSIZE);
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_q15_to_q7(src_buf_q15,result_q7, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_q15_to_q7: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));	
#ifdef PRINT_OUTPUT
  PRINT_Q(result_q7,MAX_BLOCKSIZE);
#endif  
  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_q15_to_q31(src_buf_q15,result_q31, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_q15_to_q31: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));	
#ifdef PRINT_OUTPUT
  PRINT_Q(result_q31,MAX_BLOCKSIZE);
#endif



/*convert q31*/

  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_q31_to_float(src_buf_q31,result_f32, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_q31_to_float: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));	
#ifdef PRINT_OUTPUT
  PRINT_F32(result_f32,MAX_BLOCKSIZE);
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_q31_to_q7(src_buf_q31,result_q7, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_q31_to_q7: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));	
#ifdef PRINT_OUTPUT
  PRINT_Q(result_q7,MAX_BLOCKSIZE);
#endif  
  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_q31_to_q15(src_buf_q31,result_q15, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_q31_to_q15: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));	
#ifdef PRINT_OUTPUT
  PRINT_Q(result_q15,MAX_BLOCKSIZE);
#endif

  printf("End\n");
  return 0 ;
}

 

