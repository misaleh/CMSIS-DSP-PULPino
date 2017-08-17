#include "riscv_math.h"
#include "gpio.h" //for indication for benchmarking
#include "utils.h"
#include "string_lib.h"
#include "bar.h"
#include <stdio.h>
#include "bench.h"

#define EVENT_ID 0x00  /*number of cycles ID for benchmarking*/

#define PRINT_F32(X) printf("\n"); for(int i =0 ; i < (X.numRows)*(X.numCols) ; i++) printf("%d  ",(int)(X.pData[i])); \
printf("\n\n")
#define PRINT_Q(X) printf("\n"); for(int i =0 ; i < (X.numRows)*(X.numCols) ; i++) printf("0x%X  ",X.pData[i]); \
printf("\n\n")
#define PRINTCOMP_F32(X) printf("\n"); for(int i =0 ; i < 2*(X.numRows)*(X.numCols) ; i++) printf("%d  ",(int)(X.pData[i])); \
printf("\n\n")
#define PRINTCOMP_Q(X) printf("\n"); for(int i =0 ; i < 2*(X.numRows)*(X.numCols) ; i++) printf("0x%X  ",X.pData[i]); \
printf("\n\n")


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
void perf_enable_id( int eventid){
  cpu_perf_conf_events(SPR_PCER_EVENT_MASK(eventid));
  cpu_perf_conf(SPR_PCMR_ACTIVE | SPR_PCMR_SATURATE);
};
void riscv_mat_init_f64(riscv_matrix_instance_f64*, uint16_t , uint16_t , float64_t*);
/*  4*4  */
float32_t A_f32_4_4[16] =
{
  1.0,     25.1,      4.0,     8.0, 
  3.0,     5.0,     7.0,    -3.0,
  134.0,     1.5,      128.0,      14.0,
  0.0,     -16.0,     128.0,    123.5,
};
/*   4*4   */
float32_t B_f32_4_4[16] =
{
  20,     19.1,      94.0,     -31.0,
  513.2,    11.0,     57.0,    5.0,
  1.0,     12.5,      8.0,      3.0,
  -22.0,     19.0,     2.0,    0.5,
};
/*   4*4  complex */
float32_t AComp_f32_4_4[32] =
{
  1.0,     25.1,      4.0,     8.0,  1.0,     25.1,      4.0,     8.0,
  3.0,     5.0,     7.0,    -3.0,  3.0,     5.0,     7.0,    -3.0,
  134.0,     1.5,      128.0,      14.0, 134.0,     1.5,      128.0,      14.0,
  0.0,     -16.0,     128.0,    123.5, 0.0,     -16.0,     128.0,    123.5,
};
/*   4*4  complex */
float32_t BComp_f32_4_4[32] =
{
  20,     19.1,      94.0,     -31.0,
  513.2,    11.0,     57.0,    5.0,
  1.0,     12.5,      8.0,      3.0,
  -22.0,     19.0,     2.0,    0.5,
};

/*   4*4   */
q15_t A_q15_4_4[16] =
{
  0xA901,     0x5428,      0x0054,     0x6A01,
  0x2E16,     0x51A6,      0xFF22,     0x7090,
  0x0472,     0x77A3,      0x560A,     0x1983,
  0x2255,     0x2D29,      0xA59F,     0x29FF,
};
/*   4*4   */
q15_t B_q15_4_4[16] =
{
  0x0153,     0x7F46,      0x0329,     0x8018,
  0x0922,     0x7746,      0xF717,    0x5403,
  0xA3A2,     0x217F,      0x0836,     0x2D5C,
  0xB292,     0x3C00,      0x02E2,    0x335F,
};

q31_t AComp_q31_4_4[32] =
{
  0xA9010A51,     0x54282240,      0x0054F136,     0x6A012293,  0xA9010A51,     0x54282240,      0x0054F136,     0x6A012293,
  0x2E1643A2,     0x51A61994,      0xFF229416,     0x709001A9,  0x2E1643A2,     0x51A61994,      0xFF229416,     0x709001A9,
  0x0472BB12,     0x77A318AC,      0x560A1791,     0x19830000,  0x0472BB12,     0x77A318AC,      0x560A1791,     0x19830000,
  0x2255BCFD,     0x2D29FD66,      0xA59FC1B2,     0x29FFFF12,  0x2255BCFD,     0x2D29FD66,      0xA59FC1B2,     0x29FFFF12,
};
/*   4*4   */
q31_t BComp_q31_4_4[32] =
{
  0x0153F112,     0x7F46C637,      0x03298086,    0x80181973,  0x0153F112,     0x7F46C637,      0x03298086,    0x80181973,
  0x0922CE02,     0x7746747F,      0xF7174539,    0x540322D2,  0x0922CE02,     0x7746747F,      0xF7174539,    0x540322D2,
  0xA3A25419,     0x217FCD1F,      0x08361234,    0x2D5CAAAA,  0xA3A25419,     0x217FCD1F,      0x08361234,    0x2D5CAAAA,
  0xB2928745,     0x3C00308A,      0x02E28051,    0x335F8088,  0xB2928745,     0x3C00308A,      0x02E28051,    0x335F8088,
};

q15_t AComp_q15_4_4[32] =
{
  0xA901,     0x5428,      0x0054,     0x6A01,0xA901,     0x5428,      0x0054,     0x6A01,
  0x2E16,     0x51A6,      0xFF22,     0x7090,0x2E16,     0x51A6,      0xFF22,     0x7090,
  0x0472,     0x77A3,      0x560A,     0x1983,  0x0472,     0x77A3,      0x560A,     0x1983,
  0x2255,     0x2D29,      0xA59F,     0x29FF,  0x2255,     0x2D29,      0xA59F,     0x29FF,
};
/*   4*4   */
q15_t BComp_q15_4_4[32] =
{
  0x0153,     0x7F46,      0x0329,     0x8018,  0x0153,     0x7F46,      0x0329,     0x8018, 
  0x0922,     0x7746,      0xF717,    0x5403, 0x0922,     0x7746,      0xF717,    0x5403,
  0xA3A2,     0x217F,      0x0836,     0x2D5C, 0xA3A2,     0x217F,      0x0836,     0x2D5C,
  0xB292,     0x3C00,      0x02E2,    0x335F,0xB292,     0x3C00,      0x02E2,    0x335F, 
};

q31_t A_q31_4_4[16] =
{
  0xA9010A51,     0x54282240,      0x0054F136,     0x6A012293,
  0x2E1643A2,     0x51A61994,      0xFF229416,     0x709001A9,
  0x0472BB12,     0x77A318AC,      0x560A1791,     0x19830000,
  0x2255BCFD,     0x2D29FD66,      0xA59FC1B2,     0x29FFFF12,
};
/*   4*4   */
q31_t B_q31_4_4[16] =
{
  0x0153F112,     0x7F46C637,      0x03298086,    0x80181973,
  0x0922CE02,     0x7746747F,      0xF7174539,    0x540322D2,
  0xA3A25419,     0x217FCD1F,      0x08361234,    0x2D5CAAAA,
  0xB2928745,     0x3C00308A,      0x02E28051,    0x335F8088,
};


float64_t A_f64_4_4[16] =
{
  1.0,     25.1,      4.0,     8.0,
  3.0,     5.0,     7.0,    -3.0,
  134.0,     1.5,      128.0,      14.0,
  0.0,     -16.0,     128.0,    123.5,
};
float32_t Result_f32_4_4[16];
float32_t ResultComp_f32_4_4[32];
q15_t Result_q15_4_4[16];
q15_t ResultComp_q15_4_4[32];
q31_t Result_q31_4_4[16];
q31_t ResultComp_q31_4_4[32];
q15_t scratch_q15[16];
q15_t scratchComp_q15[32];
float32_t Result_f64_4_4[16];
riscv_status status ;
float32_t scale_f32 = 2.5;
q15_t scale_q15 = 0x12B3;
q31_t scale_q31 = 0x12C3F762;
int32_t main(void)
{

/*Init Matrices*/
  riscv_matrix_instance_f32 MatA_f32_4_4;     
  riscv_matrix_instance_f32 MatB_f32_4_4;      
  riscv_matrix_instance_f32 MatResult_f32_4_4;
  riscv_matrix_instance_f32 MatAComp_f32_4_4;     
  riscv_matrix_instance_f32 MatBComp_f32_4_4;      
  riscv_matrix_instance_f32 MatResultComp_f32_4_4;
  riscv_matrix_instance_q15 MatA_q15_4_4;     
  riscv_matrix_instance_q15 MatB_q15_4_4;      
  riscv_matrix_instance_q15 MatResult_q15_4_4;
  riscv_matrix_instance_q15 MatAComp_q15_4_4;     
  riscv_matrix_instance_q15 MatBComp_q15_4_4;      
  riscv_matrix_instance_q15 MatResultComp_q15_4_4;
  riscv_matrix_instance_q31 MatA_q31_4_4;     
  riscv_matrix_instance_q31 MatB_q31_4_4;      
  riscv_matrix_instance_q31 MatAComp_q31_4_4;     
  riscv_matrix_instance_q31 MatBComp_q31_4_4; 
  riscv_matrix_instance_q31 MatResult_q31_4_4;
  riscv_matrix_instance_q31 MatResultComp_q31_4_4;
  riscv_matrix_instance_f64 MatA_f64_4_4;      
  riscv_matrix_instance_f64 MatResult_f64_4_4;

  riscv_mat_init_f32(&MatA_f32_4_4, 4, 4, (float32_t *)A_f32_4_4);
  riscv_mat_init_f32(&MatB_f32_4_4, 4, 4, (float32_t *)B_f32_4_4);
  riscv_mat_init_f32(&MatResult_f32_4_4, 4, 4,(float32_t *)Result_f32_4_4 );
  riscv_mat_init_f32(&MatAComp_f32_4_4, 4, 4, (float32_t *)AComp_f32_4_4);
  riscv_mat_init_f32(&MatBComp_f32_4_4, 4, 4, (float32_t *)BComp_f32_4_4);
  riscv_mat_init_f32(&MatResultComp_f32_4_4, 4, 4,(float32_t *)ResultComp_f32_4_4 );
  riscv_mat_init_q15(&MatA_q15_4_4, 4, 4, (q15_t *)A_q15_4_4);
  riscv_mat_init_q15(&MatB_q15_4_4, 4, 4, (q15_t *)B_q15_4_4);
  riscv_mat_init_q15(&MatAComp_q15_4_4, 4, 4, (q15_t *)AComp_q15_4_4);
  riscv_mat_init_q15(&MatBComp_q15_4_4, 4, 4, (q15_t *)BComp_q15_4_4);
  riscv_mat_init_q15(&MatResult_q15_4_4, 4, 4,(q15_t *)Result_q15_4_4 );
  riscv_mat_init_q15(&MatResultComp_q15_4_4, 4, 4,(q15_t *)ResultComp_q15_4_4 );
  riscv_mat_init_q31(&MatA_q31_4_4, 4, 4, (q31_t *)A_q31_4_4);
  riscv_mat_init_q31(&MatB_q31_4_4, 4, 4, (q31_t *)B_q31_4_4);
  riscv_mat_init_q31(&MatAComp_q31_4_4, 4, 4, (q31_t *)AComp_q31_4_4);
  riscv_mat_init_q31(&MatBComp_q31_4_4, 4, 4, (q31_t *)BComp_q31_4_4);
  riscv_mat_init_q31(&MatResult_q31_4_4, 4, 4,(q31_t *)Result_q31_4_4 );
  riscv_mat_init_q31(&MatResultComp_q31_4_4, 4, 4,(q31_t *)ResultComp_q31_4_4 );
  riscv_mat_init_f64(&MatA_f64_4_4, 4, 4, (float64_t *)A_f64_4_4);
  riscv_mat_init_f64(&MatResult_f64_4_4, 4, 4,(float64_t *)Result_f64_4_4 );
/*Tests*/

/*Add*/

  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_mat_add_f32(&MatA_f32_4_4,&MatB_f32_4_4,&MatResult_f32_4_4);
  perf_stop();
  printf("riscv_mat_add_f32: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  PRINT_F32(MatResult_f32_4_4);
#endif

  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_mat_add_q15(&MatA_q15_4_4,&MatB_q15_4_4,&MatResult_q15_4_4);
  perf_stop();
  printf("MatA_q15_4_4: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  PRINT_Q(MatResult_q15_4_4);
#endif

  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_mat_add_q31(&MatA_q31_4_4,&MatB_q31_4_4,&MatResult_q31_4_4);
  perf_stop();
  printf("MatA_q31_4_4: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  PRINT_Q(MatResult_q31_4_4);
#endif

/*complex multiplication*/

  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_mat_cmplx_mult_f32(&MatAComp_f32_4_4,&MatBComp_f32_4_4,&MatResultComp_f32_4_4);
  perf_stop();
  printf("riscv_mat_cmplx_mult_f32: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  PRINTCOMP_F32(MatResultComp_f32_4_4);
#endif

  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_mat_cmplx_mult_q15(&MatAComp_q15_4_4,&MatBComp_q15_4_4,&MatResultComp_q15_4_4,scratchComp_q15);
  perf_stop();
  printf("riscv_mat_cmplx_mult_q15: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  PRINTCOMP_Q(MatResultComp_q15_4_4);
#endif

  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_mat_cmplx_mult_q31(&MatAComp_q31_4_4,&MatBComp_q31_4_4,&MatResultComp_q31_4_4);
  perf_stop();
  printf("riscv_mat_cmplx_mult_q31: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  PRINTCOMP_Q(MatResultComp_q31_4_4);
#endif

/*inverse*/

  perf_reset();
  perf_enable_id(EVENT_ID);	
  status = riscv_mat_inverse_f32(&MatA_f32_4_4,&MatResult_f32_4_4);
  perf_stop();
  printf("riscv_mat_inverse_f32: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  PRINT_F32(MatResult_f32_4_4);
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  status = riscv_mat_inverse_f64(&MatA_f64_4_4,&MatResult_f64_4_4);
  perf_stop();
  printf("riscv_mat_inverse_f64: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  PRINT_F32(MatResult_f64_4_4);
#endif

/*multiplication*/

  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_mat_mult_f32(&MatA_f32_4_4,&MatB_f32_4_4,&MatResult_f32_4_4);
  perf_stop();
  printf("riscv_mat_mult_f32: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  PRINT_F32(MatResult_f32_4_4);
#endif

  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_mat_mult_q15(&MatA_q15_4_4,&MatB_q15_4_4,&MatResult_q15_4_4,scratch_q15);
  perf_stop();
  printf("riscv_mat_mult_q15: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  PRINT_Q(MatResult_q15_4_4);
#endif

  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_mat_mult_q31(&MatA_q31_4_4,&MatB_q31_4_4,&MatResult_q31_4_4);
  perf_stop();
  printf("riscv_mat_mult_q31: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  PRINT_Q(MatResult_q31_4_4);
#endif

/*fast multiplication*/

  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_mat_mult_fast_q15(&MatA_q15_4_4,&MatB_q15_4_4,&MatResult_q15_4_4,scratch_q15);
  perf_stop();
  printf("riscv_mat_mult_fast_q15: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  PRINT_Q(MatResult_q15_4_4);
#endif

  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_mat_mult_fast_q31(&MatA_q31_4_4,&MatB_q31_4_4,&MatResult_q31_4_4);
  perf_stop();
  printf("riscv_mat_mult_fast_q31: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  PRINT_Q(MatResult_q31_4_4);
#endif

/*scale*/

  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_mat_scale_f32(&MatA_f32_4_4,scale_f32,&MatResult_f32_4_4);
  perf_stop();
  printf("riscv_mat_scale_f32: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  PRINT_F32(MatResult_f32_4_4);
#endif

  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_mat_scale_q15(&MatA_q15_4_4,scale_q15,3,&MatResult_q15_4_4);
  perf_stop();
  printf("riscv_mat_scale_q15: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  PRINT_Q(MatResult_q15_4_4);
#endif

  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_mat_scale_q31(&MatA_q31_4_4,scale_q31,3,&MatResult_q31_4_4);
  perf_stop();
  printf("riscv_mat_scale_q31: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  PRINT_Q(MatResult_q31_4_4);
#endif

/*subtract*/

  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_mat_sub_f32(&MatA_f32_4_4,&MatB_f32_4_4,&MatResult_f32_4_4);
  perf_stop();
  printf("riscv_mat_sub_f32: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  PRINT_F32(MatResult_f32_4_4);
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_mat_sub_q15(&MatA_q15_4_4,&MatB_q15_4_4,&MatResult_q15_4_4);
  perf_stop();
  printf("riscv_mat_sub_q15: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  PRINT_Q(MatResult_q15_4_4);
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_mat_sub_q31(&MatA_q31_4_4,&MatB_q31_4_4,&MatResult_q31_4_4);
  perf_stop();
  printf("riscv_mat_sub_q31: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  PRINT_Q(MatResult_q31_4_4);
#endif

/*transpose*/

  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_mat_trans_f32(&MatA_f32_4_4,&MatResult_f32_4_4);
  perf_stop();
  printf("riscv_mat_trans_f32: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  PRINT_F32(MatResult_f32_4_4);
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_mat_trans_q15(&MatA_q15_4_4,&MatResult_q15_4_4);
  perf_stop();
  printf("riscv_mat_trans_q15: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  PRINT_Q(MatResult_q15_4_4);
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_mat_trans_q31(&MatA_q31_4_4,&MatResult_q31_4_4);
  perf_stop();
  printf("riscv_mat_trans_q31: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  PRINT_Q(MatResult_q31_4_4);
#endif

  printf("End\n");
  return 0 ;
}

void riscv_mat_init_f64(
  riscv_matrix_instance_f64 * S,
  uint16_t nRows,
  uint16_t nColumns,
  float64_t * pData)
{
  S->numRows = nRows;
  S->numCols = nColumns;
  S->pData = pData;
}




