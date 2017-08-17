#include <math.h>
#include <stdio.h>
#include "riscv_math.h"
#include "gpio.h" 
#include "utils.h"
#include "string_lib.h"
#include "bar.h"
#include "bench.h"

#define PRINT_F32(X,Y) printf("\n"); for(int i =0 ; i < (Y); i++) printf("%d  ",(int)(X[i]*100)); \
printf("\n\n")
#define PRINT_Q(X,Y) printf("\n"); for(int i =0 ; i < (Y); i++) printf("0x%X  ",X[i]); \
printf("\n\n")
//#define PRINT_OUTPUT  /*for testing functionality for each function, removed while benchmarking*/
#define MAX_BLOCKSIZE     32
#define EVENT_ID 0x00  /*number of cycles ID for benchmarking*/
/*
  Benchmarking now is done with bench library, these were old functions
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
 float32_t srcA_buf_f32[MAX_BLOCKSIZE] =
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

 float32_t srcB_buf_f32[MAX_BLOCKSIZE] =
{
   1.7491401329284098,  0.1325982188803279,   0.3252281811989881,
  -0.7938091410349637,  0.3149236145048914,  -0.5272704888029532,
   0.9322666565031119,  1.1646643544607362,  -2.0456694357357357,
  -0.6443728590041911,  1.7410657940825480,   0.4867684246821860,
   1.0488288293660140,  1.4885752747099299,   1.2705014969484090,
  -1.8561241921210170,  2.1343209047321410,  1.4358467535865909,
  -0.9173023332875400, -1.1060770780029008,   0.8105708062681296,
   0.6985430696369063, -0.4015827425012831,   1.2687512030669628,
  -0.7836083053674872,  0.2132664971465569,   0.7878984786088954,
   0.8966819356782295, -0.1869172943544062,   1.0131816724341454,
   0.2484350696132857,  0.0596083377937976
};
 q7_t srcA_buf_q7[MAX_BLOCKSIZE] =
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

 q7_t srcB_buf_q7[MAX_BLOCKSIZE] =
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

 q15_t srcA_buf_q15[MAX_BLOCKSIZE] =
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

 q15_t srcB_buf_q15[MAX_BLOCKSIZE] =
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
 q31_t srcA_buf_q31[MAX_BLOCKSIZE] =
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
 q31_t srcB_buf_q31[MAX_BLOCKSIZE] =
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


float32_t result_f32[MAX_BLOCKSIZE];  /* Final ouput */
q7_t result_q7[MAX_BLOCKSIZE];
q15_t result_q15[MAX_BLOCKSIZE];
q31_t result_q31[MAX_BLOCKSIZE];

q63_t result_q63_1;
q31_t result_q31_1;
float32_t result_f32_1;

int i = 0 ;
int32_t main(void)
{

 /*Init*/ 
 /* 
  set_pin_function(5, FUNC_GPIO);
  set_gpio_pin_direction(5, DIR_OUT);
  set_pin_function(6, FUNC_GPIO);
  set_gpio_pin_direction(6, DIR_OUT);
  CLR_GPIO_5() ;
  CLR_GPIO_6() ;
*/
/*Tests*/
/*abs*/


  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_abs_f32( srcA_buf_f32,result_f32,MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_abs_f32: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));

#ifdef PRINT_OUTPUT
  PRINT_F32(result_f32,MAX_BLOCKSIZE);
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_abs_q7(srcA_buf_q7,result_q7,MAX_BLOCKSIZE);
  printf("riscv_abs_q7: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
  perf_stop();
#ifdef PRINT_OUTPUT
  PRINT_Q(result_q7,MAX_BLOCKSIZE);
/*Results from pulpino were compared with results from RISCV m4 and also were checked by hand*/
  printf("\nCorrect answer:\n");
  printf("0x75 0x13 0x15 0x44 0x11 0x1C 0x52 0x0B 0x7D 0x6F 0x33  0x25 0x1A 0x4D 0x6F 0x26 0x01 0x0F 0x54 0x66 0x76 0x54 0x79  0x36 0x22 0x33 0x1F 0x61 0x54 0x35 0x6F 0x49");
  printf("\n");
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_abs_q15(srcA_buf_q15,result_q15,MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_abs_q15: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  PRINT_Q(result_q15,MAX_BLOCKSIZE);
  printf("\nCorrect answer:\n");
  printf("0x7512 0x1375 0x1565 0x44C3 0x1188 0x1CA1 0x5264 0x0B20 0x7CFB 0x6EEE 0x3399 0x2518 0x1AB2 0x4D01 0x6F23 0x26FF 0x0121 0x0EDD 0x53B9 0x6688 0x76A2 0x5476 0x78AA 0x36B3 0x2245 0x3373 0x1E57 0x610A 0x5419 0x3501 0x6F00 0x4469");
  printf("\n");
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_abs_q31(srcA_buf_q31,result_q31,MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_abs_q31: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  PRINT_Q(result_q31,MAX_BLOCKSIZE);
  printf("\nCorrect answer:\n");
  printf("0x75122542 0x1375A138 0x15654473 0x44C34463 0x118854C3 0x1CA18291 0x5264E236 0x0B200590 0x7CFA6589 0x6EED68ED 0x3399D123 0x2518BBD1 0x1AB2037A 0x4D01D1F1 0x6F237531 0x26FF5569 0x01213159 0x0EDC3289 0x53B8ABAA 0x6688A12E 0x76A2FF96 0x54761269 0x78A92BC8 0x36B3B697 0x22459643 0x3373C368 0x1E56FE2D 0x610A0540 0x54199643 0x350112B5 0x6EFFFBCA 0x4469AA15");
  printf("\n");
#endif

/*add*/

  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_add_f32(srcA_buf_f32, srcB_buf_f32, result_f32, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_add_f32: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  PRINT_F32(result_f32,MAX_BLOCKSIZE);
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_add_q7(srcA_buf_q7, srcB_buf_q7, result_q7, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_add_q7: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
/*negatvie number are printed with FFFFFF before it because it is signed*/
  PRINT_Q(result_q7,MAX_BLOCKSIZE);
  printf("\nCorrect answer:\n");
  printf("0x7F 0x26 0x2A 0x7F 0x22 0x38 0x7F 0x16 0x80 0x80 0x66 0x4A 0x34 0x7F 0x7F 0x4C 0x2 0xE2 0x80 0x7F 0x7F 0x7F 0x80 0x6C 0x44 0x66 0xC2 0x7F 0x7F 0x6A 0x80 0x7F");
  printf("\n");
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_add_q15(srcA_buf_q15, srcB_buf_q15, result_q15, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_add_q15: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));

#ifdef PRINT_OUTPUT
  PRINT_Q(result_q15,MAX_BLOCKSIZE);
  printf("\nCorrect answer:\n");
  printf("0x7FFF 0x26EA 0x2ACA 0x7FFF 0x2310 0x3942 0x7FFF 0x1640 0x8000 0x8000 0x6732 0x4A30 0x3564 0x7FFF 0x7FFF 0x4DFE 0x242 0xE246 0x8000 0x7FFF 0x7FFF 0x7FFF 0x8000 0x6D66 0x448A 0x66E6 0xC352 0x7FFF 0x7FFF 0x6A02 0x8000 0x7FFF");
  printf("\n");
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_add_q31(srcA_buf_q31, srcB_buf_q31, result_q31, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_add_q31: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  PRINT_Q(result_q31,MAX_BLOCKSIZE);
  printf("\nCorrect answer:\n");
  printf("0x7FFFFFFF 0x26EB4270 0x2ACA88E6 0x7FFFFFFF 0x2310A986 0x39430522 0x7FFFFFFF 0x16400B20 0x80000000 0x80000000 0x6733A246 0x4A3177A2 0x356406F4 0x7FFFFFFF 0x7FFFFFFF 0x4DFEAAD2 0x24262B2 0xE2479AEE 0x80000000 0x7FFFFFFF 0x7FFFFFFF 0x7FFFFFFF 0x80000000 0x6D676D2E 0x448B2C86 0x66E786D0 0xC35203A6 0x7FFFFFFF 0x7FFFFFFF 0x6A02256A 0x80000000 0x7FFFFFFF");
  printf("\n");
#endif

/*dot product*/
  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_dot_prod_q15(srcA_buf_q15,srcB_buf_q15,MAX_BLOCKSIZE,&result_q63_1);
  perf_stop();
  printf("riscv_dot_prod_q15: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
 /*int pointer to print the long long on two halfs as printf doesn't print long long*/
  int * ptr = &result_q63_1;  /*for printing only*/
  ptr++;
  printf("0x%X",*(ptr--));
  printf("%X ",*(ptr));
  printf("\n");
  printf("\nCorrect answer:\n");
  printf("0x2B4B3CAF7");
  printf("\n");
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_dot_prod_q31(srcA_buf_q31,srcB_buf_q31,MAX_BLOCKSIZE,&result_q63_1);
  perf_stop();
  printf("riscv_dot_prod_q31: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  ptr = &result_q63_1;   /*for printing only*/
  ptr++;
  printf("0x%X",*(ptr--));
  printf("%X ",*(ptr));
  printf("\n");
  printf("\nCorrect answer:\n");
  printf("0xAD2DD380D7254");
  printf("\n");
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_dot_prod_f32(srcA_buf_f32,srcB_buf_f32,MAX_BLOCKSIZE,&result_f32_1);
  perf_stop();
  printf("riscv_dot_prod_f32: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  printf("%d\n",1000*((int)result_f32_1));
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_dot_prod_q7(srcA_buf_q7,srcB_buf_q7,MAX_BLOCKSIZE,&result_q31_1);
  perf_stop();
  printf("riscv_dot_prod_q7: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  printf("0x%X ",result_q31_1);  
  printf("\n");
  printf("\nCorrect answer:\n");
  printf("0x0002B420");
  printf("\n");
#endif

/*Mult*/
  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_mult_f32(srcA_buf_f32, srcB_buf_f32, result_f32, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_mult_f32: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  PRINT_F32(result_f32,MAX_BLOCKSIZE);
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_mult_q31(srcA_buf_q31, srcB_buf_q31, result_q31, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_mult_q31: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  PRINT_Q(result_q31,MAX_BLOCKSIZE);
  printf("\nCorrect answer:\n");
  printf("0x6B132EAF 0x02F557F6 0x03938A93 0x24F0A28B 0x0266C7B8 0x066774EA 0x35099158 0x00F788F7 0x7A070E8E 0x6021C4AE 0x14CD4B7F 0x0AC0515C 0x059148FB 0x2E54309C 0x607F8913 0x0BE1980C 0x00028D60 0x01B9A5DB 0x36C2890E 0x522252B8 0x6DF556CD 0x37BB6513 0x71BE11AC 0x17609E61 0x092D1DA8 0x14AEA869 0x07310244 0x499130BD 0x37419A55 0x15F2E380 0x6041F8B2 0x24909BF0");
  printf("\n");
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_mult_q15(srcA_buf_q15, srcB_buf_q15, result_q15, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_mult_q15: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  PRINT_Q(result_q15,MAX_BLOCKSIZE);
  printf("\nCorrect answer:\n");
  printf("0x6B12 0x02F5 0x0393 0x24F0 0x0266 0x0667 0x3508 0x00F7 0x7A08 0x6022 0x14CC 0x0ABF 0x0591 0x2E53 0x607E 0x0BE1 0x0002 0x01B9 0x36C2 0x5221 0x6DF3 0x37BB 0x71BF 0x1760 0x092C 0x14AE 0x0731 0x4991 0x3740 0x15F2 0x6042 0x248F");
  printf("\n");
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_mult_q7(srcA_buf_q7, srcB_buf_q7, result_q7, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_mult_q7: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  PRINT_Q(result_q7,MAX_BLOCKSIZE);
  printf("\nCorrect answer:\n");
  printf("0x6A 0x02 0x03 0x24 0x02 0x06 0x34 0x00 0x7A 0x60 0x14 0x0A 0x05 0x2E 0x60 0x0B 0x00 0x01 0x37 0x51 0x6C 0x37 0x72 0x16 0x09 0x14 0x07 0x49 0x37 0x15 0x60 0x29 ");
  printf("\n");
#endif

  printf("End\n");

  return 0 ;
}
