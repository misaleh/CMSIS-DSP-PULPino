#include <math.h>
#include <stdio.h>
#include "riscv_math.h"
#include "gpio.h"
#include "utils.h"
#include "string_lib.h"
#include "bar.h"
#include "bench.h"

#define EVENT_ID 0x00  /*number of cycles ID for benchmarking*/

#define PRINT_F32(X,Y) printf("\n"); for(int i =0 ; i < (Y); i++) printf("%d  ",(int)(100*X[i])); \
printf("\n\n")
#define PRINT_Q(X,Y) printf("\n"); for(int i =0 ; i < (Y); i++) printf("0x%X  ",X[i]); \
printf("\n\n")
//#define PRINT_OUTPUT  /*for testing functionality for each function, removed while benchmarking*/
#define MAX_BLOCKSIZE     32
#define NUM_STAGES 1
#define CONV_BLOCKSIZE   ((2*MAX_BLOCKSIZE) - 1)
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

 float64_t srcA_buf_f64[MAX_BLOCKSIZE] =
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
q31_t coeffs_q31[5] =  {0x7531, 0x3344,0xAA76, 0x01A1, 0x5C00};  /*   b10, b11, b12, a11, a12 */
q63_t state_q63[4]; /*state variables x[n-1], x[n-2], y[n-1], y[n-2]*/
q31_t state_q31[4]; /*state variables x[n-1], x[n-2], y[n-1], y[n-2]*/

float32_t coeffs_f32[5] =  {0.75, -0.4, 0.6, 0.8, -0.45};  /*   b10, b11, b12, a11, a12 */
float32_t state_f32[4]; /*state variables x[n-1], x[n-2], y[n-1], y[n-2]*/

float64_t coeffs_f64[5] =  {0.75, -0.4, 0.6, 0.8, -0.45};  /*   b10, b11, b12, a11, a12 */
float64_t state_f64[4]; /*state variables x[n-1], x[n-2], y[n-1], y[n-2]*/

q15_t coeffs_q15[6] =  {0x75,0 ,0x33,0xAA, 0x01, 0x5C};  /*   b10, b11, b12, a11, a12 */
q15_t state_q15[4]; /*state variables x[n-1], x[n-2], y[n-1], y[n-2]*/


q15_t scratch1[3*MAX_BLOCKSIZE-2]; /*scartch buffer for internal computation of opt functions*/
q15_t scratch2[MAX_BLOCKSIZE]; 



float32_t result_f32[MAX_BLOCKSIZE]; 
q7_t result_q7[MAX_BLOCKSIZE];
q15_t result_q15[MAX_BLOCKSIZE];
q31_t result_q31[MAX_BLOCKSIZE];
float64_t result_f64[32];
/*conv results*/
float32_t conv_result_f32[2*MAX_BLOCKSIZE-1]; 
q7_t conv_result_q7[2*MAX_BLOCKSIZE-1];
q15_t conv_result_q15[2*MAX_BLOCKSIZE-1];
q31_t conv_result_q31[2*MAX_BLOCKSIZE-1];
/*biquad df1 variables*/
riscv_biquad_cas_df1_32x64_ins_q31 S32x64_q31;
riscv_biquad_casd_df1_inst_f32 Sdf1_f32;
riscv_biquad_casd_df1_inst_q31 Sdf1_q31;
riscv_biquad_casd_df1_inst_q15 Sdf1_q15;
/*biquad df2 variables*/
riscv_biquad_cascade_df2T_instance_f32 Sdf2_f32;
riscv_biquad_cascade_df2T_instance_f64 Sdf2_f64;
riscv_biquad_cascade_stereo_df2T_instance_f32 Sdf2stereo_f32;
int i = 0 ;
int32_t main(void)
{

 /*Init*/ 
/*biquad df1 inits*/
  riscv_biquad_cas_df1_32x64_init_q31( &S32x64_q31, NUM_STAGES, coeffs_q31, state_q63,  0);
  riscv_biquad_cascade_df1_init_f32(&Sdf1_f32,NUM_STAGES, coeffs_f32,state_f32);
  riscv_biquad_cascade_df1_init_q15(&Sdf1_q15, NUM_STAGES,coeffs_q15,state_q15,0);
  riscv_biquad_cascade_df1_init_q31(&Sdf1_q31, NUM_STAGES,coeffs_q31,state_q31,0);
/*biquad df2 inits*/
  riscv_biquad_cascade_df2T_init_f32(&Sdf2_f32,NUM_STAGES,coeffs_f32,state_f32);
  riscv_biquad_cascade_df2T_init_f64(&Sdf2_f64,NUM_STAGES,coeffs_f64,state_f64);
  riscv_biquad_cascade_stereo_df2T_init_f32(&Sdf2stereo_f32,NUM_STAGES,coeffs_f32,state_f32);
/*Tests*/
/*biquad df1*/

  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_biquad_cas_df1_32x64_q31( &S32x64_q31 ,srcA_buf_q31,result_q31, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_biquad_cas_df1_32x64_q31: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));	
#ifdef PRINT_OUTPUT
  PRINT_Q(result_q31,MAX_BLOCKSIZE);
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_biquad_cascade_df1_f32(&Sdf1_f32,srcA_buf_f32,result_f32,MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_biquad_cascade_df1_f32: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));	
#ifdef PRINT_OUTPUT
  PRINT_F32(result_f32,MAX_BLOCKSIZE);
#endif

  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_biquad_cascade_df1_q15(&Sdf1_q15,srcA_buf_q15,result_q15,MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_biquad_cascade_df1_q15: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));	
#ifdef PRINT_OUTPUT
  PRINT_Q(result_q15,MAX_BLOCKSIZE);
#endif

  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_biquad_cascade_df1_q31(&Sdf1_q31,srcA_buf_q31,result_q31,MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_biquad_cascade_df1_q31: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));	
#ifdef PRINT_OUTPUT
  PRINT_Q(result_q31,MAX_BLOCKSIZE);
#endif
	

  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_biquad_cascade_df1_fast_q15(&Sdf1_q15,srcA_buf_q15,result_q15,MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_biquad_cascade_df1_fast_q15: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));	
#ifdef PRINT_OUTPUT
  PRINT_Q(result_q15,MAX_BLOCKSIZE);
#endif 

  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_biquad_cascade_df1_fast_q31(&Sdf1_q31,srcA_buf_q31,result_q31,MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_biquad_cascade_df1_fast_q31: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));	
#ifdef PRINT_OUTPUT
  PRINT_Q(result_q31,MAX_BLOCKSIZE);
#endif



/*biquad df2*/

  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_biquad_cascade_df2T_f32(&Sdf2_f32,srcA_buf_f32,result_f32,MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_biquad_cascade_df2T_f32: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));	
#ifdef PRINT_OUTPUT
  PRINT_F32(result_f32,MAX_BLOCKSIZE);
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_biquad_cascade_df2T_f64(&Sdf2_f64,srcA_buf_f64,result_f64,MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_biquad_cascade_df2T_f64: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));	
#ifdef PRINT_OUTPUT
  PRINT_F32(result_f64,MAX_BLOCKSIZE);
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_biquad_cascade_stereo_df2T_f32(&Sdf2stereo_f32,srcA_buf_f32,result_f32,MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_biquad_cascade_stereo_df2T_f32: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));	
#ifdef PRINT_OUTPUT
  PRINT_F32(result_f32,MAX_BLOCKSIZE);
#endif



  printf("End\n");

  return 0 ;
}
